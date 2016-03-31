from __future__ import absolute_import
from .interpreter import Interpreter
import os
__version__ = '0.0.1'
current_path = os.path.split(__file__)[0]
path_to_ntp = os.path.join(current_path, '..')
__path__.append(path_to_ntp)


__all__ = ['Interpreter']

