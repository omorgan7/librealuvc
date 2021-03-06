from setuptools import setup, find_packages
from setuptools.dist import Distribution

# _version.py should be generated by running find_librs_version.py and copied to pyrealuvc folder
from pyrealuvc._version import __version__

import os
import io

package_name = "pyrealuvc"
package_data = {}

print("version = ", __version__)

def load_readme():
     with io.open('README.rst', encoding="utf-8") as f:
        return f.read()

if os.name == 'posix':
    package_data[package_name] = ['*.so']
else:
    package_data[package_name] = ['*.pyd', '*.dll']

# This creates a list which is empty but returns a length of 1.
# Should make the wheel a binary distribution and platlib compliant.
class EmptyListWithLength(list):
    def __len__(self):
        return 1


setup(
    name=package_name,
    version=__version__,
    author='Intel(R) RealSense(TM) / Leap Motion',
    author_email='rcownie@leapmotion.com',
    url='https://github.com/IntelRealSense/librealsense',
    scripts=[],
    license='Apache License, Version 2.0',
    description='Python Wrapper for librealuvc',
    long_description=load_readme(),
    install_requires=[],
    classifiers=[
        'Development Status :: 3 - Alpha',
        'Environment :: Console',
        'Intended Audience :: Developers',
        'Intended Audience :: Education',
        'Intended Audience :: Science/Research',
        'License :: OSI Approved :: Apache Software License',
        'Operating System :: Microsoft :: Windows',
        'Operating System :: POSIX',
        'Operating System :: Unix',
        'Programming Language :: Python',
        'Programming Language :: Python :: 3',
        'Programming Language :: Python :: 3.6',
        'Topic :: Multimedia :: Video',
        'Topic :: Scientific/Engineering',
        'Topic :: Scientific/Engineering :: Human Machine Interfaces',
        'Topic :: Scientific/Engineering :: Image Recognition',
        'Topic :: Software Development',
        'Topic :: Software Development :: Libraries :: Application Frameworks'
        ],
    packages=find_packages(exclude=['third_party', 'docs', 'examples']),
    include_package_data=True,
    ext_modules=EmptyListWithLength(),
    package_data=package_data
)
