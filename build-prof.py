#!/usr/bin/python
import re, json, os, subprocess, argparse, sys, shutil
from subprocess import Popen, PIPE
from datetime import datetime

"""Open Speed Shop Test Script
by Patrick Romero 4/11/16
contact snixromero@gmail.com for questions"""

module = None #need to be global

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
module = glo['module']

def mpi_root(mpi_module):
	mpi = module_kind(mpi_module)
	if mpi == 'mpt':
		return ('-DMPT_DIR', '$MPI_ROOT')
	if mpi == 'mpich':
		return ('-DMPICH_DIR', '$I_MPI_ROOT')
	if mpi == 'openmpi':
		print 'doing some sketchy stuff'
		#openmpi dosent set a root variable so we need to figure that out manually
		module('load', mpi_module)
		path = os.environ['PATH']
		paths = path.split[':']
		for p in paths:
			if re.match(r'.*open_?mpi.*', p, re.M|re.I):
				mpi_root = os.path.split(p)[0]
				print mpi_root
		return ('-DOPENMPI_DIR', mpi_root)

def module_kind(module):
	if re.match(r'.*gcc.*', module, re.M|re.I):
		return 'gcc'
	if re.match(r'.*intel.*', module, re.M|re.I):
		if re.match(r'.*mpi.*', module, re.M|re.I):
			return 'mpich'
		if re.match(r'.*math.*', module, re.M|re.I):
			return 'math-intel'
		return 'intel'
	if re.match(r'.*mpt.*', module, re.M|re.I):
		return 'mpt'
	if re.match(r'.*open_?mpi.*', module, re.M|re.I):
		return 'openmpi'
	if re.match(r'.*mvapich.*', module, re.M|re.I):
		return 'mvapich'
	if re.match(r'.*mvapich2.*', module, re.M|re.I):
		return 'mvapich2'
	if re.match(r'.*math.*', module, re.M|re.I):
		return 'math'
	return ''

def create_profiles(filename, mpi_modules, cc_modules, other_modules):
	"""create the default profiles.json file"""

	print "generated default environment file profiles.json"
	print "please look for any inconsistencies in this file"

	compatability_set = [ #to be expanded
		('mpt', 'intel', 'math-intel'),
		('mpich', 'intel', 'math-intel'),
		('openmpi', 'gnu', 'math'),
		('mpt', 'gnu', 'math') ]

	profiles = []

	for mpi_module in mpi_modules:
		for cc_module in cc_modules:
			for other_module in other_modules:
				module_key = (module_kind(mpi_module), module_kind(cc_module), module_kind(other_module))
				if not module_key in compatability_set:
					continue
				else:
					if module_kind(mpi_module) == 'mpt':
						mpi_command = 'mpiexec_mpt -np 8'
					else:
						mpi_command = 'mpiexec -np 8'

					prof = { "cc": module_kind(cc_module),
							"mpi_imp": module_kind(mpi_module),
							"cc_module": cc_module,
							"mpi_module": mpi_module,
							'mpi_driver': mpi_command,
							"cmake_flag_var": mpi_root(mpi_module),
							"mpi_cmake_flag": "-DMPT_DIR",
							"other_modules":["math/intel_mkl_default"]}

	pfile = open(filename,'w')
	pfile.write(json.dumps(profiles,sort_keys=True, indent=2, separators=(',', ': ')))
	pfile.close()

def main(args=None, error_func=None):

	parser = argparse.ArgumentParser(description='build profiles for test-script.py')
	parser.add_argument('--mpi-modules', nargs='+', type=str,
		help='array of mpi modules')
	parser.add_argument('--mpi-module', nargs='?', type=str,
		help='single mpi module')
	parser.add_argument('--cc-modules', nargs='+', type=str,
		help='array of compiler modules')
	parser.add_argument('--cc-module', nargs='?', type=str,
		help='single compiler module')
	parser.add_argument('--other-modules', nargs='+', type=str,
		help='array of other modules that may be needed by some profiles')

	args = parser.parse_args(sys.argv[1:] if args is None else args)

	mpi_modules = []
	cc_modules = []
	other_modules = []

	if args.mpi_modules:
		mpi_modules.extend(args.mpi_modules)
	if args.mpi_module:
		mpi_modules.append(args.mpi_modules)
	if args.cc_modules:
		cc_modules.extend(args.cc_modules)
	if args.cc_module:
		cc_modules.append(args.cc_modules)
	if args.other_modules:
		other_modules.extend(args.other_modules)

	if args.mpi_modules:
		print 'mpi modules are ' + str(args.mpi_modules)

	create_profiles('profiles.json', mpi_modules, cc_modules, other_modules)










main()
