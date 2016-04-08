from __future__ import absolute_import
from .interpreter import Interpreter
from .interpreter import run_cli
from .interpreter import run_command

import os
__version__ = '0.0.1'
current_path = os.path.split(__file__)[0]
path_to_ntp = os.path.join(current_path, '..')
__path__.append(path_to_ntp)


__all__ = ['Interpreter', 'run_cli', 'run_command']
