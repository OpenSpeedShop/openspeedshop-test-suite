#!/usr/bin/python2
import re
import json
import os
import subprocess
from subprocess import Popen, PIPE
import argparse
import sys
import shutil
from datetime import datetime

"""Open Speed Shop Test Script
by Patrick Romero 4/11/16
contact snixromero@gmail.com for questions"""

##### naming utility functions #####
def get_db_name(cmd):
	testname = os.path.split(cmd[1])[1]
	expname = cmd[0][3:]
	return  testname + '-' + expname + '.openss'

def test_obj(env, file_name):
    test = {}
    lst = file_name.split('-')
    test['exe'] = os.path.join(os.path.join(env['install_dir'], env['bin_dir']), file_name)
    #test['exe'] = file_name
    test['name'] = lst[1]
    test['mpi_driver'] = lst[3] if len(lst) > 3 else ''
    test['compiler'] = lst[-1]
    #determine appropriate collectors, according to the old test-tool.sh
    coll = ['hwc', 'hwctime', 'hwcsamp', 'pcsamp', 'usertime']
    if test['mpi_driver'] != '':
        coll.extend(['mpi', 'mpit'])
    ##
    if env['dynamic_cbtf']:
        coll.append('mem')
        if test['mpi_driver'] != '':
            coll.append('mpip')
    test['collectors'] = coll
    return test
           

        
#####################################

def build_tests(env):
    base_dir = os.getcwd()
    os.chdir(env['build_scripts_dir'])
    compilers = env['compilers']
    log = []
    for cc in compilers:
        cwd = os.getcwd()
        buildcmd = 'run_build_' + cc +'.sh' #identify build script
        buildcmd = os.path.join(cwd,buildcmd)
        print buildcmd
        p = Popen(buildcmd)
        p.wait()
        if p.returncode != 0:
            log.append('build script ' + buildcmd + ' failed')
        else:
            log.append('build script ' + buildcmd + ' succeeded')

    print '\tSUMMARY:'
    for l in log:
        print l
    os.chdir(base_dir)

def create_env(filename):
    """create the default environment file"""
    print "generated default environment file env.json"
    print "please edit this to have correct values"
    env = {'bin_dir':'bin',
        'test_data_dir':'test_data',
        'build_scripts_dir':'build_scripts',
        'src_dir':'src',
        'dynamic_cbtf':False,
        'job_controller':'raw',
        'compilers':['gnu'] }
    envfile = open(filename,'w')
    envfile.write(json.dumps(env,sort_keys=True, indent=2, separators=(',', ': ')))
    envfile.close()

def read_env(filename):
    """Read in environment variables from env.json"""
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
    if not os.path.isdir(d):
        try: os.mkdir(d)
        except:
            print 'failed to create directory ' + os.path.join(os.getcwd(), d)
            print 'do you have write permissions?'
            return 1
    os.chdir(d)

def run_tests(env, tests, is_baseline):
    base_dir = os.getcwd()
    mk_cd('test_data')
    if is_baseline:
        mk_cd('baseline')
    else:
        mk_cd('results')
    folder = str(datetime.now())
    folder = folder.split()[0] + '_' + folder.split()[1]
    mk_cd(folder)

    job_cont = env['job_controller']
    if job_cont == 'raw':	
        return raw_job_controller(env, tests)
    elif job_cont == 'moab':
        return raw_job_controller(env, tests)
    elif job_cont == 'slurm':
        return raw_job_controller(env, tests)
    elif job_cont == 'pbs':
        return raw_job_controller(env, tests)
    else:
        print 'invalid job controller type ' + job_cont
    os.chdir(base_dir)
		
def moab_job_controller(env, tests):
    '''
        #PBS -l nodes=4:ppn=12
        #PBS -l walltime=1:00:00:00
        #PBS -A xyz-123-ab
        #PBS -j oe
        #PBS -V
        #PBS -N jobname
         
        import os
        import subprocess
         
        os.chdir(os.getenv('PBS_O_WORKDIR', '/home/username/your_project_name'))
        os.environ['IPATH_NO_CPUAFFINITY'] = '1'
        os.environ['OMP_NUM_THREADS'] = '12'
         
        subprocess.call("mpiexec -n 4 -npernode 1 ./yourcode > results.txt", shell=True)
         
        # Some other work in pure Python
    '''
    pass #need to implement


def slurm_job_controller(env, tests):

    pass #need to implement

def pbs_job_controller(env, tests):
    '''
    # Example PBS cluster job submission in Python
     
    from popen2 import popen2
    import time
     
    # If you want to be emailed by the system, include these in job_string:
    #PBS -M your_email@address
    #PBS -m abe  # (a = abort, b = begin, e = end)
     
    # Loop over your jobs
    for i in range(1, 10):
     
        # Open a pipe to the qsub command.
        output, input = popen2('qsub')
         
        # Customize your options here
        job_name = "my_job_%d" % i
        walltime = "1:00:00"
        processors = "nodes=1:ppn=1"
        command = "./my_program -n %d" % i
     
        job_string = """#!/bin/bash
        #PBS -N %s
        #PBS -l walltime=%s
        #PBS -l %s
        #PBS -o ./output/%s.out
        #PBS -e ./error/%s.err
        cd $PBS_O_WORKDIR
        %s""" % (job_name, walltime, processors, job_name, job_name, command)
         
        # Send job_string to qsub
        input.write(job_string)
        input.close()
         
        # Print your job and the system response to the screen as it's submitted
        print job_string
        print output.read()
         
        time.sleep(0.1)
    '''
    pass

def raw_job_controller(env, tests):
    failed = []
    base_dir = os.getcwd()
    
    for t in tests: #loop through all tests
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

			

def compare_tests(env, tests):
    base_dir = os.getcwd()
    try: os.chdir(env['test_data_dir'])
    except:
            print 'failed to locate test_data directory, exiting...'
            return 1
    failed = [] #list of error messages
    big_log = [] #log of all tests
    variance = 0.005 #allowed variance in percent
                   #allows two values to differ by up to 0.5%
    for t in tests:
        x = t['exe']
        for c in t['collectors']:
            baseline_file = get_db_name(['oss'+c,x])
            baseline_dir = get_recent('baseline')
            baseline = os.path.join(baseline_dir, baseline_file)

            results_file = get_db_name(['oss'+c,x])
            results_dir = get_recent('results')
            results = os.path.join(results_dir, results_file)

            if not os.path.isfile(baseline):
                err = 'failed to locate baseline file ' + baseline
                #print err
                failed.append(err)
                continue
            if not os.path.isfile(results):
                err = 'failed to locate results file ' + results
                #print err
                failed.append(err)
                continue

            compare_metric = 'percent'
            if c in ['mem', 'mpi', 'io', 'iot', 'pthreads', 'mpit']:
                compare_metric = 'counts'
            elif c == 'hwsamp':
                compare_metric = 'allEvents'

            cmd = 'osscompare ' + str('\"' + baseline + ',' + results + '\" ' + compare_metric)
            #print str(cmd)
            p = Popen(cmd, stdout=PIPE, stderr=PIPE, shell=True)
            p.wait()
            output = p.stdout.read()
            matches = re.finditer(r'^(\s*)(\d*\.\d*)(\s*)(\d*\.\d*)(\s+)([_\w]+)',output, re.M|re.I)
            log = "------------------------------------------------------\n"
            log += 'compare for ' + c + ' on ' + x + ':\n\n'
            all_passed = True
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
            else:
                failed.append('test ' + c + ' on ' + x + ' failed a variance test')
            log += "------------------------------------------------------\n"
            big_log.append(log)
            #compare_file = get_comp_file_name(cmd)
            if p.returncode != 0:
                err = 'osscompare failed: ' + cmd[0] + ' ' + cmd[1] + ' ' + cmd[2]
                print err
                failed.append(err)
                try: os.rm(compare_file)
                except: pass
                continue
    os.chdir(base_dir)

    for log in big_log: #summarize results
        print log

    print '\n\tSUMMARY:\n'
    if len(failed) > 0:
        print "tests failed:"
        for f in failed:
            print f
    else:
        print "successfully compared all tests:"
        for t in tests:
            print t['name']

def clean_tests(clean_baseline):
    cmd = 'rm -rf test_data/results/*'
    if clean_baseline:
        cmd = 'rm -rf test_data/baseline/*'
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
            e = datetime.strptime(entry, fmt_string)
            r = datetime.strptime(recent, fmt_string)
            if e > r:
                recent = entry
    return os.path.join(d,recent)
    

def main(args=None, error_func=None):

    openss_experiments = ["pcsamp", "usertime", "hwc", "hwcsamp", "hwctime",
        "io", "iop", "iot", "mem", "mpi", "mpip", "mpit", "mpiotf", "pthreads",
        "fpe", "cuda"]

    parser = argparse.ArgumentParser(description='Open Speed Shop test utily')

    parser.add_argument('-e', nargs='?', 
        help='use an alternate env file (default=$INSTALL/env.json)')

    parser.add_argument('--create-env', nargs='?', const='env.json',
        metavar='ENV_FILE', help='create a default environment file (default=env.json)')

    parser.add_argument('--create-baseline', nargs='+', metavar='TEST_NAME',
        help='create baseline for tests')

    parser.add_argument('--create-baseline-all', action='store_true',
        help='create baseline for all tests')

    parser.add_argument('--build-tests', action='store_true',
        help='build tests according to parameters in env.json file')
    parser.add_argument('--run-tests', nargs='+', metavar='TEST_NAME', help='run tests')

    parser.add_argument('--run-all', action='store_true' , help='run all tests')

    parser.add_argument('--compare-tests', nargs='+', metavar='TEST_NAME', help='compare tests')
    parser.add_argument('--compare-all', action='store_true' , help='compare all tests of most recent run')

    parser.add_argument('-b', nargs='?',
        help='use specific baseline folder in compare, default is the most recent')
    parser.add_argument('-r', nargs='?',
        help='use specific results folder in compare, default is the most recent')
    

    parser.add_argument('--clean-run', action='store_true' , help='clean run data')
    parser.add_argument('--clean-all', action='store_true' , help='clean run and baseline data')
    #parser.add_argument('--experiments', nargs='+', metavar='EXPERIMENT',
    #choices=openss_experiments, help='openss experiments to run on tests; pcsamp, hwc, etc.')

    args = parser.parse_args(sys.argv[1:] if args is None else args)

    ####################### BEGIN MAIN #####################
    install_dir = os.path.dirname(os.path.abspath(__file__)) #get install location
    base_dir = os.getcwd()

    if args.create_env:
        create_env(args.create_env)
        envfile = args.create_env
        exit(0)

    if args.e:
        envfile = args.e
        #read in the environment data. eventually this should be autoconfigured
        env = read_env(envfile)
        os.chdir(install_dir)
    else:
        os.chdir(install_dir)
        env = read_env('env.json')

    env['install_dir'] = install_dir #used to build absolute paths

    if args.build_tests:
        build_tests(env)
        exit(0)
    
    tests = []
    bd = os.path.join(base_dir, env['bin_dir'])
    for root, dirs, files in os.walk(bd):
        for entry in files:
            if os.access(os.path.join(root,entry), os.X_OK): #check that file has x bit
                tests.append(test_obj(env,entry))
    
    if args.clean_run:
        clean_tests(env, False)
    elif args.clean_all:
        clean_tests(env, True)

    #now run whichever tests are specified
    if args.run_tests:
        tests_to_run = []
        for test in tests:
            if test['exe'] in args.run_tests:
                tests_to_run.append(test)
        run_tests(env, tests_to_run, False) #run tests, no baseline mode
        exit(0)

    elif args.run_all:
        run_tests(env, tests, False) #run tests, no baseline mode
        exit(0)

    #run whichever tests are specified in baseline mode (exclusive to run_tests)
    elif args.create_baseline:
        tests_to_run = []
        for test in tests:
            if test['name'] in args.run_tests:
                tests_to_run.append(test)
        run_tests(env, tests_to_run, True) #run tests, no baseline mode
        exit(0)

    elif args.create_baseline_all:
        run_tests(env, tests, True) #run tests, no baseline mode
        exit(0)

    os.chdir(base_dir)
    
    #compare which test results are specified
    if args.compare_tests:
        tests_to_comp = []
        for test in tests:
            if test['name'] in args.compare_tests:
                tests_to_comp.append(test)
        compare_tests(env, tests_to_comp)
    elif args.compare_all:
        compare_tests(env, tests)




main()
