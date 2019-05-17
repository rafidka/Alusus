import os
import shutil
import argparse
from install_all_deps import install_all_deps
from install_dep import get_local_site_packages
from threading import Lock
import sys

global_args = None

def _process_args():
    global global_args
    if global_args:
        return
    global_args = dict()

    global_args['releaseInstallPath'] = os.path.join('/', 'opt', 'Alusus') if os.name == "posix" else os.path.join('/', 'Alusus')
    parser = argparse.ArgumentParser(add_help=False,
                                     formatter_class=argparse.RawTextHelpFormatter)
    parser.add_argument('-h', '--help', action='help', default=argparse.SUPPRESS,
                        help='Show this help message and exit.')
    parser.add_argument("--btype", metavar="BUILD_TYPE", help="Set the build type of Alusus:\n" +
                        "    d [default]: debug build.\n" +
                        "    r: release build.\n" +
                        "    p: release and package build.",
                        choices=["d", "r", "p"], required=False, default="d")
    parser.add_argument("--bloc", metavar="BUILD_LOCATION", help="Set the build location (default is the sources root directory).",
                        required=True, default=None)
    parser.add_argument("--iloc", metavar="INSTALL_LOCATION", help="Set the install location (default is the value of \"--bloc\").",
                        required=False, default=None)
    parser.add_argument("-g", action="store_true", help="Set the install location globally on the current system"
                                                        " (located in \"{}\" - MAY REQUIRE ROOT PRIVILEGE)."
                                                        " This will be overridden by the value of \"--iloc\" option.".format(
                                                            global_args['releaseInstallPath']),
                        required=False, default=False)
    args = parser.parse_args()

    global_args['buildType'] = "debug"
    global_args['createPackages']  = "no"
    if args.btype == "r":
        global_args['buildType'] = "release"
    elif args.btype == "p":
        global_args['buildType'] = "release"
        global_args['createPackages']  = "yes"

    if args.bloc:
        global_args['productPath'] = os.path.realpath(args.bloc)
        if not os.path.isdir(global_args['productPath']):
            os.makedirs(global_args['productPath'])
        global_args['buildsPath'] = os.path.join(global_args['productPath'], "Builds")
        global_args['buildPath'] = os.path.join(
            global_args['buildsPath'], global_args['buildType'][0].upper() + global_args['buildType'][1:])
        global_args['depsPath'] = os.path.join(global_args['buildsPath'], "Dependencies")
        global_args['pythonDepsPath'] = os.path.join(global_args['depsPath'], "PythonDeps")
        if not os.path.exists(global_args['pythonDepsPath']):
            os.makedirs(global_args['pythonDepsPath'])
        global_args['localSitePackagesPath'] = get_local_site_packages(global_args['pythonDepsPath'])
        global_args['packagesPath'] = os.path.join(global_args['buildsPath'], "Packages")
        global_args['installPath'] = os.path.join(global_args['productPath'], "LocalInstall")

        # Install all deps.
        install_all_deps(global_args['pythonDepsPath'], True)
    
    if args.g:
        global_args['installPath'] = global_args['releaseInstallPath']
        shutil.rmtree(global_args['installPath'], ignore_errors=True)
        os.makedirs(global_args['installPath'])

    if args.iloc:
        global_args['installPath'] = os.path.realpath(args.iloc)

_process_args()
