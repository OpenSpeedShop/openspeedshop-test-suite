#!/usr/bin/python
import re, json, os, subprocess, argparse, sys, shutil
from subprocess import Popen, PIPE
from datetime import datetime

"""Open Speed Shop Test Script
by Patrick Romero 4/11/16
contact snixromero@gmail.com for questions"""

module = None #need to be global

##### naming utility functions #####
def get_db_name(cmd):
        """find the name of the database generated by the openspeedshop command"""

	testname = os.path.split(cmd[1])[1]
	expname = cmd[0][3:]
        if expname in ['mpi', 'mpit']:
            mpi_imp = str(os.path.split(cmd[1])[1]).split('-')[2]
	    return  testname + '-' + expname + '-' + mpi_imp + '.openss'
	return  testname + '-' + expname + '.openss'

def test_obj(env, file_name):
    """create a dictionary holding information about a test"""

    test = {}
    lst = file_name.split('-')
    if len(lst) < 3:
	return None
    test['exe'] = os.path.join(os.path.join(os.path.join(env['install_dir'], env['bin_dir']), 'bin'), file_name)
    test['name'] = lst[1]
    test['mpi_imp'] = lst[2] if len(lst) > 3 else ''
    test['mpi_module'] =  ''
    for p in env['profiles']:
	if p['mpi_imp'] == test['mpi_imp']:
	    test['mpi_module'] = p['mpi_module']
	    test['mpi_driver'] = p['mpi_driver']
    test['compiler'] = lst[-1]
    #determine appropriate collectors, according to the old test-tool.sh
    coll = ['hwc', 'hwctime', 'hwcsamp', 'pcsamp', 'usertime']
    if test['mpi_imp'] != '':
        coll.extend(['mpi', 'mpit'])
    ##
    if env['dynamic_cbtf']:
        coll.append('mem')
        if test['mpi_imp'] != '':
            coll.append('mpip')
    test['collectors'] = coll
    return test

#####################################

def run_cmake(build_dir, flags):
    """run a cmake sequence, build dir, cd, cmake, make, cleanup..."""
    base_dir = os.getcwd()
    try: shutil.rmtree(build_dir)
    except: pass
    mk_cd(build_dir)
    f = lambda x: str(x[0] + '=' + x[1])
    flags = list(map(f,flags))
    cmd = ['cmake', '..'] + flags
    print str(cmd)
    for c in [cmd, ['make', 'clean'], ['make'], ['make', 'install']]:
        p = Popen(c)
        p.wait()
        if p.returncode != 0:
            print c[0] + ' failed:'
            print str(c)
            exit(1)

    os.chdir(base_dir)
    shutil.rmtree(build_dir)
    

def build_tests(env):
    """run cmake to build the tests with each of the compilers specified in the env"""
    cmake_flags = [('-DCMAKE_INSTALL_PREFIX', env['bin_dir']),
        ('-DCMAKE_BUILD_TYPE', 'None'),
        ('-DCMAKE_CXX_FLAGS', '-g -O2'),
        ('-DCMAKE_C_FLAGS', '-g -O2')]
        #('-DOPENMPI_DIR', env['openmpi_root'])]

    for profile in env['profiles']:
	#TODO purge and load modules
	module('purge')
	for m in profile['other_modules']:
	    module('load', m.encode('ascii','ignore'))
	module('load', profile['cc_module'].encode('ascii','ignore'))
	module('load', profile['mpi_module'].encode('ascii','ignore'))
	if profile['cmake_flag_var'][0] == '$': #treat as an environment variable
	    profile_flags = [(profile['mpi_cmake_flag'],os.environ[profile['cmake_flag_var'][1:]])] #strip the $ sign
	else: #treat as a path
	    profile_flags = [(profile['mpi_cmake_flag'],profile['cmake_flag_var'])]
	if 'intel' == profile['cc']:
	    intel_flags = [('-DCMAKE_CXX_COMPILER', 'icpc'),
		('-DCMAKE_C_COMPILER', 'icc'),
		('-DLIBIOMP_DIR', env['ompt_root']),
		('-DBUILD_COMPILER_NAME', 'intel')]
	    run_cmake('intel_build', cmake_flags + profile_flags + intel_flags)
	    
	if 'pgi' == profile['cc']:
	    pgi_flags = [('-DBUILD_COMPILER_NAME', 'pgi')]
	    run_cmake('pgi_build', cmake_flags + profile_flags + pgi_flags)
	    
	if 'gnu' == profile['cc']:
            gnu_flags = [('-DBUILD_COMPILER_NAME', 'gnu')]
            run_cmake('gnu_build', cmake_flags + profile_flags + gnu_flags)

################################################
### the functions create_env and create_profiles create some default settings
## that will generally need to be modified.
## env contains information about the system, and
## profiles contains the compiler/mpi schemes that will be used to compile the tests
def find_modules(modules):
	modules_home = os.environ['MODULEPATH']

	my_modules = []
	for root, dirs, files in os.walk(modules_home):
   		for f in files:
			ff = open(os.path.join(root,f))
			module_src = ff.read()
			for m in modules:
				pattern = re.compile(r'.*set\s+' + re.escape(mpi) + r'.*', re.M|re.I)
				if pattern.search(module_src):
					my_modules.append(m)
	return my_modules



def create_env(filename):
    """create the default environment file"""

    print "generated default environment file env.json"
    print "please edit this to have correct values"
    env = {'bin_dir':'bin',
        'test_data_dir':'test_data',
        'src_dir':'src',
        'dynamic_cbtf':False,
        'oss_version': 'oss_offline-2.2.2',
	'ompt_root':'/nobackupnfs2/jgalarow/krellroot_v2.2.6',
        'job_controller':'raw',
        'acceptable_variance':10.0,
	'max_concurrent_jobs':25,
	'openss_module':'home4/jgalarow/privatemodules/osscbtf226',
	'input_dir':'input_files' }
    envfile = open(filename,'w')
    envfile.write(json.dumps(env,sort_keys=True, indent=2, separators=(',', ': ')))
    envfile.close()

def create_profiles(filename):
    """create the default profiles.jsonn file"""

    print "generated default environment file profiles.json"
    print "please edit this to have correct values"

    profiles = [{ "cc": "intel",
	"mpi_imp": "mpt",
	"cc_module": "comp-intel/2016.2.181",
	"mpi_module": "mpi-sgi/mpt.2.12r26",
        'mpi_driver':'mpiexec_mpt -np 8',
	"cmake_flag_var": "$MPI_ROOT",
	"mpi_cmake_flag": "-DMPT_DIR",
	"other_modules":["math/intel_mkl_default"]}]

    pfile = open(filename,'w')
    pfile.write(json.dumps(profiles,sort_keys=True, indent=2, separators=(',', ': ')))
    pfile.close()

def read_profiles(filename):
    '''Read in profiles information from filename'''

    try:
	envfile = open(filename,'r')
    except:
        print "profiles file not found, please create one using --create-prof"
        print "then edit this file (profiles.json) to have correct values"
        print "exiting..."
        exit()
		
    env = json.load(envfile)
    envfile.close()
    return env

   

def read_env(filename):
    '''Read in environment information from env.json'''

    try:
	envfile = open(filename,'r')
    except:
        print "environment file not found, please create one using --create-env"
        print "then edit this file (env.json) to have correct values"
        print "exiting..."
        exit()
		
    env = json.load(envfile)
    envfile.close()
    return env

def mk_cd(d):
    #make a dir and cd to it, safely
    if not os.path.isdir(d):
        try: os.mkdir(d)
        except:
            print 'failed to create directory ' + os.path.join(os.getcwd(), d)
            print 'do you have write permissions?'
            return 1
    os.chdir(d)

def run_tests(env, tests, is_baseline):
    '''run a list of tests, store the data by data and is_baseline'''

    base_dir = os.getcwd()
    input_dir = os.path.join(base_dir,env['input_dir'])
    mk_cd(env['test_data_dir'])
    mk_cd(env['oss_version'])
    if is_baseline:
        mk_cd('baseline')
    else:
        mk_cd('results')
    folder = str(datetime.now())
    folder = folder.split()[0] + '_' + folder.split()[1]
    mk_cd(folder)

    #copy the 3 input files needed to the cwd
    env['input_dir'] = os.path.join(base_dir, env['input_dir'])
    for f in ['input', 'matmul_input.txt', 'stress.input']:
        input_file = os.path.join(env['input_dir'], f)
        shutil.copyfile(input_file, os.path.join(os.getcwd(), f))

    job_cont = env['job_controller']
    if job_cont == 'raw':	
        return raw_job_controller(env, tests)
    elif job_cont == 'moab':
        return moab_job_controller(env, tests)
    elif job_cont == 'slurm':
        return slurm_job_controller(env, tests)
    elif job_cont == 'pbs':
        return pbs_job_controller(env, tests)
    else:
        print 'invalid job controller type ' + job_cont
    os.chdir(base_dir)
		
def moab_job_controller(env, tests):
    pass #need to implement


def slurm_job_controller(env, tests):
    pass #need to implement

def pbs_job_controller(env, tests):

    j_ids = [] #pbs job ids, for limiting the number of concurrent jobs via dependencies
    max_jobs = env['max_concurrent_jobs']
    num_jobs = 0
    print str(tests)
    for t in tests: #loop through all tests
	
	#create a unique run folder so that topology files do not interfere. cd to this
	#dir and run the test there. then have the pbs script move its files to the dest dir
	#and clean up after itself.
	base_dir = os.getcwd()
	run_dir = t['name'] + '-' + t['mpi_imp'] + '-' + t['compiler']
	stdoutfile = os.path.join(base_dir,run_dir + 'stdout.txt')
	stderrfile = os.path.join(base_dir,run_dir + 'stderr.txt')
	mk_cd(run_dir)
	cleanup_line = 'mv '+ base_dir + '/' + run_dir + '/* ' + str(base_dir) #bash code to save files and cleanup
	cleanup_line += '\n' + 'rm -rf ' + os.path.join(base_dir,str(run_dir)) + '\n'

	#copy input files. will be ignored if not necesarry
        for f in ['input', 'matmul_input.txt', 'stress.input']:
            input_file = os.path.join(env['input_dir'], f)
            shutil.copyfile(input_file, os.path.join(os.getcwd(), f))

	#locate any input files that need to be piped for specific tests
	input_pipe = ''
	if t['name'] == 'matmul':
	    input_pipe = ' < ' + os.path.join(os.getcwd(), 'matmul_input.txt')
	elif t['name'] == 'openmp_stress':
	    input_pipe = ' < ' + os.path.join(os.getcwd(), 'stress.input')
	elif t['name'] == 'lulesh' or t['name'] == 'lulesh203' :
	    input_pipe = ' -i 30 '

	string1 = \
        '#PBS -S /bin/csh\n\
#  PBS script file\n\
#   submit this script using the command:\n\
#       qsub run.pbs\n\
\n\
#PBS -l select=2:ncpus=16:model=has\n\
#PBS -N test-suite-' + t['name'] + '-' + t['mpi_imp'] + '-' + t['compiler'] + '\n' + \
'#PBS -l walltime=1:00:00\n\
#PBS -j oe\n\
#PBS -o ' + stdoutfile + '\n\
#PBS -e ' + stderrfile + '\n\
#PBS -m bea\n\
#PBS -q debug\n\
\n\
source $MODULESHOME/init/csh\n\
setenv OMP_NUM_THREADS 2\n'
	
        if t['mpi_imp'] != '': #check if this an mpi test

	    oss_cmds = ''
            for c in t['collectors']: #loop through all collectors to run
                #run each collector on the test program
                #also build the mpirun command
		oss_cmd = 'oss' + c + ' \"' + t['mpi_driver'] + ' ' + t['exe'] + input_pipe + '\"\n'
		oss_cmds += oss_cmd

            jobscript = string1 + \
'setenv OPENSS_MPI_IMPLEMENTATION ' + t['mpi_imp'] + '\n\
module load modules ' + env['openss_module'] + '\n\
module load modules ' + t['mpi_module'] + '\n\
echo "showing environment for debugging purposes"\n\
echo " =========================="\n\
setenv CBTF_MPI_IMPLEMENTATION ' + t['mpi_imp'] + '\n\
env \n\
echo " =========================="\n\
# run case\n\
' + \
oss_cmds + \
'echo " "\n\
echo "finished run, cleaning up..."\n' + cleanup_line + '\n\
echo " "\n\
echo " =========================="\n\
'
	    file = open('temp_pbs_run.pbs', 'w')
	    file.write(jobscript)
	    file.close()
	    if num_jobs < max_jobs: #submit the first jobs
		pbs_cmd = ['qsub', 'temp_pbs_run.pbs']
	    else: #create a dependency chain of jobs to avoid overloading the job controller
		pbs_cmd = ['qsub', '-W depend=afterany:'+j_ids[num_jobs-max_jobs][:-1], 'temp_pbs_run.pbs']
	    print 'created job script, submitting with ' + str(pbs_cmd)
	    print jobscript
	    print '---------------------------------'
	    p = Popen(pbs_cmd, stdout=PIPE)
	    p.wait()
	    jid = p.stdout.read()
	    j_ids.append(jid)
	    num_jobs += 1

	else: #not an mpi test
	    oss_cmds = ''
	    for c in t['collectors']:	
                oss_cmd = 'oss' + c + ' \"' + t['exe'] + input_pipe + '\"\n'
		oss_cmds += oss_cmd
	    jobscript = string1 + \
'module load modules ' + env['openss_module'] + '\n\
# run case\n\
' + \
oss_cmds + \
'echo " "\n\
echo "finished run, cleaning up..."\n' + cleanup_line + '\n\
echo " "\n\
echo " =========================="\n\
'
	    file = open('temp_pbs_run.pbs', 'w')
	    file.write(jobscript)
	    file.close()
	    if num_jobs < max_jobs: #submit the first jobs
		pbs_cmd = ['qsub', 'temp_pbs_run.pbs']
	    else: #create a dependency chain of jobs to avoid overloading the job controller
		pbs_cmd = ['qsub', '-W depend=afterany:'+j_ids[num_jobs-max_jobs][:-1], 'temp_pbs_run.pbs']
	    print 'created job script, submitting with ' + str(pbs_cmd)
	    print jobscript
	    print '---------------------------------'
	    p = Popen(pbs_cmd, stdout=PIPE)
	    p.wait()
	    jid = p.stdout.read()
	    j_ids.append(jid)
	    num_jobs += 1
	os.chdir(base_dir) #always cd back to correct dir
    print'_______________________________\n'
    print 'Created and submitted all job scripts, please allow some time for them to complete\n'
	




def raw_job_controller(env, tests):
    ''' dispatch jobs as you would on your laptop or pc'''

    failed = []
    base_dir = os.getcwd()
    os.environ['OMP_NUM_THREADS'] = '2'
    
    for t in tests: #loop through all tests
	module('purge')
	module('load',env['openss_module'].encode('ascii','ignore'))
        if t['mpi_imp'] != '': #check if this an mpi test
            print str(t)
	    module('load',t['mpi_module'].encode('ascii','ignore'))
            for c in t['collectors']: #loop through all collectors to run
                #run each collector on the test program
                #also build the mpirun command
                cmd = ['oss' + c, t['mpi_driver'] + ' ' + t['exe']]
                print base_dir
                print cmd[0] + ' ' + cmd[1]
                p = Popen(cmd)
                p.wait() #wait for subprocess to finish
                db_name = get_db_name(cmd) #get the name of the output file
                #OPTIONAL os.rm(input_file)
                if p.returncode != 0:
                    print 'failed to run test ' + c + ' ' + t['exe']
                    failed.append(c + ", " + t['exe'])
                try: os.rm(db_name)
                except: pass
            
        else: #not an mpi test, just run once normally
            if t['name'] == 'sweep3d': #need to move input file
                input_file = os.path.join(os.path.join(env['install_dir'], env['src_dir']), 'sweep3d/input')
                shutil.copyfile(input_file, os.path.join(os.getcwd(),'input'))

            for c in t['collectors']: #loop through all collectors to run
                #run each collector on the test program
                cmd = ['oss' + c, t['exe']]
                print base_dir
                print cmd[0] + ' ' + cmd[1]
                p = Popen(cmd)
                p.wait() #wait for subprocess to finish
                db_name = get_db_name(cmd) #get the name of the output file
            #OPTIONAL os.rm(input_file)
            if p.returncode != 0:
                print 'failed to run test ' + c + ' ' + t['exe']
                failed.append(c + ", " + t['exe'])
            try: os.rm(db_name)
            except: pass
                            
    if len(failed) > 0:
            print "failed to run tests:"
            for f in failed:
                    print f
    else:
            print "successfully ran all tests:"
            for t in tests:
                    print t['name']

def compare_tests(env, tests, args):
    '''compare the results of a list of tests, summarize'''

    module('load',env['openss_module'].encode('ascii','ignore'))
    base_dir = os.getcwd()
    try:
        os.chdir(env['test_data_dir'])
    except:
            print 'failed to locate test_data directory, exiting...'
            return 1

    results_dir = ''
    baseline_dir = ''
    try: results_dir = get_recent(os.path.join(env['oss_version'],'results'))
    except: pass
    try: baseline_dir = get_recent(os.path.join(env['oss_version'],'baseline'))
    except: pass
    #find data directory if the user specified manually
    if args.b:
        baseline_dir = args.b
    if args.r:
        results_dir = args.r
    if args.baseline_version:
        print args.baseline_version
        baseline_dir = get_recent(os.path.join(args.baseline_version,'baseline'))
    if results_dir == '' or baseline_dir == '' or \
        not os.path.isdir(results_dir) or not os.path.isdir(baseline_dir):
        print 'invalid test data directory'
        return 1

    failed = [] #list of error messages
    succeeded = [] #list of passed tests
    big_log = [] #log of all tests
    variance = env['acceptable_variance']  #allowed variance in percent

    #instead of searching for test objects, search by  db files
    pattern = re.compile(r'.*\.openss$', re.M|re.I)

    for root, dirs, files in os.walk(baseline_dir):
	for f in files:
	    if not pattern.match(str(f)):
		continue
	    results_file = os.path.join(results_dir, f)
	    baseline_file = os.path.join(baseline_dir, f)

	    filename = os.path.split(f)[1]
	    c = str(filename).split('\.')[0]
            c = c.split('-')[-2]
            compare_metric = 'percent'
            if c in ['mem', 'mpi', 'io', 'iot', 'pthreads', 'mpit']:
                compare_metric = 'counts'
            elif c == 'hwsamp':
                compare_metric = 'allEvents'

            #move results and baseline files to a temp dir
            #cd, run, cd, rm
            mk_cd('temp')
	    print os.getcwd()
	    if not os.path.isfile('../'+baseline_file):
		continue
	    if not os.path.isfile('../'+results_file):
		continue
            shutil.copyfile('../' + baseline_file, './baseline')
            shutil.copyfile('../' + results_file, './results')

            cmd = 'osscompare \"baseline,results\" ' + compare_metric
            #print str(cmd)
            p = Popen(cmd, stdout=PIPE, stderr=PIPE, shell=True)
            p.wait()
            output = p.stdout.read()
            matches = re.finditer(r'^(\s*)(\d*\.\d*)(\s*)(\d*\.\d*)(\s+)([_\w]+)',output, re.M|re.I)
            log = "------------------------------------------------------\n"
	    log += 'running tests on ' + f + '\n'
            #log += 'compare for ' + c + ' on ' + x + ':\n\n'
            all_passed = True

            os.chdir('..')
            shutil.rmtree('temp')

            print output
            for m in matches:
                func_name = m.groups()[5]
                lcount = float(m.groups()[1])
                rcount = float(m.groups()[3])
                if lcount > rcount + variance or lcount < rcount - variance:
                #if values are outside of acceptable variance, fail.
                    log += 'function values outside of acceptable variance:\n'
                    log += func_name + ' ' + str(lcount) + ' ' + str(rcount) + '\n\n'
                    all_passed = False
            if all_passed:
                log += 'all function values are within acceptable variance\n'
                succeeded.append( f + ' passed all tests')
            else:
                failed.append(f + ' failed a variance test')
            log += "------------------------------------------------------\n"
            big_log.append(log)
            #compare_file = get_comp_file_name(cmd)
            if p.returncode != 0:
                err = 'osscompare failed: ' + cmd
		err += '\n\t on files: ' + baseline_file + ', ' + results_file
                print err
                failed.append(err)
                try: os.rm(compare_file)
                except: pass
                continue
    os.chdir(base_dir)

    for log in big_log: #summarize results
        print log

    print '\n\tSUMMARY:\n'
    if len(succeeded) > 0:
        print "tests succeeded:"
        for s in succeeded:
            print s
    print "_____________________________"
    if len(failed) > 0:
        print "tests failed:"
        for f in failed:
            print f
    else:
        print "successfully compared all tests:"
        for t in tests:
            print t['name']

def clean_tests(env, clean_baseline):
    cmd = 'rm -rf ' + os.path.join(env['test_data_dir'],env['oss_version']) + '/results/*'
    if clean_baseline:
        cmd = 'rm -rf ' + env['test_data_dir'] + '/*'
    p = Popen(cmd, shell=True)
    p.wait()
    if p.returncode == 0:
        print 'cleaned test_data folder'
    else:
        print 'failed to clean test data folder, try manually?'

def get_recent(d):
    recent = None
    for root, dirs, files in os.walk(d):
        for entry in dirs:
            if not recent:
                recent = entry
            fmt_string = '%Y-%m-%d_%H:%M:%S.%f'
            try: e = datetime.strptime(entry, fmt_string)
	    except: pass
            try: r = datetime.strptime(recent, fmt_string)
	    except: pass
            if e > r:
                recent = entry
    return os.path.join(d,recent)
    
def main(args=None, error_func=None):

    parser = argparse.ArgumentParser(description='Open Speed Shop test utily')

    parser.add_argument('-e', nargs='?', 
        help='use an alternate env file (default=$INSTALL/env.json)')
    parser.add_argument('-p', nargs='?', 
        help='use an alternate profiles file (default=$INSTALL/profiles.json)')

    parser.add_argument('--create-env', nargs='?', const='env.json',
        metavar='ENV_FILE', help='create a default environment file (default=env.json)')
    parser.add_argument('--create-prof', nargs='?', const='profiles.json',
        metavar='ENV_FILE', help='create a default profiles file (default=profiles.json)')

    parser.add_argument('--create-baseline', action='store_true',
        help='create baseline for all tests')

    parser.add_argument('--build-tests', action='store_true',
        help='build tests according to parameters in env.json file')

    parser.add_argument('--run-tests', action='store_true' , help='run all tests')

    parser.add_argument('--compare-tests', action='store_true' , help='compare all tests of most recent run')

    parser.add_argument('-b', nargs='?', type=str,
        help='use specific baseline folder in compare, default is the most recent')
    parser.add_argument('-r', nargs='?', type=str,
        help='use specific results folder in compare, default is the most recent')
    parser.add_argument('--baseline-version', nargs='?', type=str,
        help='use the most recent baseline folder in this oss version to compare against')
    

    parser.add_argument('--clean-run', action='store_true' , help='clean run data')
    parser.add_argument('--clean-all', action='store_true' , help='clean run and baseline data')

    args = parser.parse_args(sys.argv[1:] if args is None else args)

    ####################### Look for module init file #############
    if not os.environ.has_key('MODULESHOME'): raise EnvironmentError('Environment variable "MODULESHOME" not found')

    # Search for these paths within the MODULESHOME directory (in this order)
    search_paths = ['init/python',
	'init/python.py',]

    # Stop at first path that exists
    for sp in search_paths:
	init_py = os.path.join( os.environ['MODULESHOME'], sp )
	if os.path.exists(init_py): break

    if not os.path.exists(init_py): raise IOError("EnvironmentModules python script was not found")

    # Execute the file
    glo = {}
    execfile(init_py, glo)
    global module
    module = glo['module']

    ####################### BEGIN MAIN #####################
    install_dir = os.path.dirname(os.path.abspath(__file__)) #get install location
    base_dir = os.getcwd()

    if args.create_env:
        create_env(args.create_env)
        envfile = args.create_env
        exit(0)

    if args.create_prof:
        create_profiles(args.create_prof)
        p_file = args.create_prof
        exit(0)

    if args.e:
        envfile = args.e
        #read in the environment data. eventually this should be autoconfigured
        env = read_env(envfile)
        os.chdir(install_dir)
    else:
        os.chdir(install_dir)
        env = read_env('env.json')
    env['bin_dir'] = os.path.join(install_dir,env['bin_dir'])

    if args.p:
        pfile = args.p
        #read in the profiles data. eventually this should be autoconfigured
        prof = read_env(pfile)
        os.chdir(install_dir)
    else:
        os.chdir(install_dir)
        prof = read_env('profiles.json')


    env['install_dir'] = install_dir #used to build absolute paths
    env['profiles'] = prof #used mostly for building

    if args.build_tests:
        build_tests(env)
        exit(0)
    
    tests = []
    #bd = os.path.join(base_dir, env['bin_dir'])
    bd = env['bin_dir']
    for root, dirs, files in os.walk(bd):
        for entry in files:
            if os.access(os.path.join(root,entry), os.X_OK): #check that file has x bit
		t = test_obj(env,entry)
		if t != None:
                	tests.append(test_obj(env,entry))
    
    if args.clean_run:
        clean_tests(env, False)
    elif args.clean_all:
        clean_tests(env, True)

    #commented out blocks allow the user to specify which tests to use
    
    #now run whichever tests are specified
    #if args.run_tests:
    #    tests_to_run = []
    #    for test in tests:
    #        if test['exe'] in args.run_tests:
    #            tests_to_run.append(test)
    #    run_tests(env, tests_to_run, False) #run tests, no baseline mode
    #    exit(0)

    if args.run_tests:
        run_tests(env, tests, False) #run tests, no baseline mode
        exit(0)

    #run whichever tests are specified in baseline mode (exclusive to run_tests)
    #elif args.create_baseline:
    #    tests_to_run = []
    #    for test in tests:
    #        if test['name'] in args.run_tests:
    #            tests_to_run.append(test)
    #    run_tests(env, tests_to_run, True) #run tests, no baseline mode
    #    exit(0)

    elif args.create_baseline:
        run_tests(env, tests, True) #run tests, no baseline mode
        exit(0)

    os.chdir(base_dir)
    
    #compare which test results are specified
    #if args.compare_tests:
    #    tests_to_comp = []
    #    for test in tests:
    #        if test['name'] in args.compare_tests:
    #            tests_to_comp.append(test)
    #    compare_tests(env, tests_to_comp)
    if args.compare_tests:
        compare_tests(env, tests, args)
            
    
########
main()
