# -*- coding: utf-8 -*-

import os
from setuptools import setup

current_dir = os.path.abspath(os.path.dirname(__file__))
version_file = os.path.join(current_dir, 'nanotec_nanolib', '__version__.py')

meta_info = {}
with open(version_file, mode='r', encoding='utf-8') as meta_file:
    exec(meta_file.read(), meta_info)

with open('README.md', mode='r', encoding='utf-8') as readme_file:
    readme = readme_file.read()

setup(
    name=meta_info['__title'],
    version=meta_info['__version'],
    description=meta_info['__description'],
    long_description=readme,
    long_description_content_type='text/markdown',
    author=meta_info['__author'],
    author_email=meta_info['__author_email'],
    url=meta_info['__url'],
    packages=['nanotec_nanolib'],
    package_data={'': ['LICENSE', 'NOTICE'], 'nanotec_nanolib': ['*.pem']},
    package_dir={'nanotec_nanolib': 'nanotec_nanolib'},
    include_package_data=True,
    python_requires=">=2.7, !=3.0.*, !=3.1.*, !=3.2.*, !=3.3.*, !=3.4.*",
    install_requires=[],
    license=meta_info['__license'],
    zip_safe=False,
    classifiers=[
        'Development Status :: 3 - Alpha',
        'Environment :: Console',
        'Intended Audience :: Developers',
        'Natural Language :: English',
        'License :: Creative Commons Attribution 3.0 Unported License. Creative Commons Attribution-NoDerivatives 4.0 International License.',
        'Operating System :: Microsoft :: Windows :: Windows 10',
        'Programming Language :: C++',
        'Programming Language :: Python :: 3.7',
        'Programming Language :: Python :: 3.8',
        'Programming Language :: Python :: 3.9',
		'Programming Language :: Python :: 3.10',
        'Programming Language :: Python :: Implementation :: CPython',
        'Programming Language :: Python :: Implementation :: PyPy',
        'Topic :: Scientific/Engineering',
        'Topic :: Scientific/Engineering :: Human Machine Interfaces',
        'Topic :: Software Development :: Libraries',
        'Topic :: System :: Hardware :: Hardware Drivers'
    ],
    extras_require={}
)
