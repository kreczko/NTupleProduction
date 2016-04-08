from __future__ import print_function
import readline
import string
import os
import shlex
import importlib
import types
import sys

PROMPT = 'ntp > '
HISTFILE = os.path.expanduser('~/.ntp_history')
COMPLETEKEY = 'tab'

CURRENT_PATH = os.path.split(__file__)[0]
PATH_TO_NTP = os.path.join(CURRENT_PATH, '..')
BASE_MODULE = 'ntp.commands'


def get_commands(command_path):
    """
        Reads the folder sub-structure of ntp/commands and 
        returns all found modules that contain a Command class.

        The folder structure 
        ntp/commands/list/X
        ntp/commands/list/Y
        ntp/commands/run/X
        is mapped onto
        {
            'ntp.commands.list.X': Command class
            'ntp.commands.list.Y': Command class
            'ntp.commands.run.X': Command class
        }

    """
    modules = {}
    for p, _, _ in os.walk(command_path):
        relative_path = os.path.relpath(p, command_path)
        # If it's the current directory, ignore
        if relative_path == '.':
            continue

        # Convert directory structure to module path
        relative_path = relative_path.replace('/', '.')
        absolute_path = '{0}.{1}'.format(BASE_MODULE, relative_path)
        try:
            mod = importlib.import_module(absolute_path)
            if hasattr(mod, 'Command'):
                if type(mod.Command) is types.ClassType:
                    modules[relative_path] = mod.Command
        except ImportError, e:
            continue

    return modules


class Interpreter():

    def __init__(self):
        self.command_path = os.path.join(CURRENT_PATH, 'commands')
        self.commands = get_commands(self.command_path)

    def complete(self, text, state):
        pass

    def execute(self, relative_path, args):
        try:
            command_class = self.commands[relative_path]
            command = command_class()
            rc = command.run(args)
        except:
            print('Command failed', file=sys.stderr)

        text = command.get_text()
        if len(text) > 0:
            print(text, end='')
            if text[len(text) - 1] != '\n':
                print()
        if rc is True:
            return 0
        return -1


def run_cli(prompt=PROMPT):
    """ sets up command line interface"""
    interpreter = Interpreter()

    readline.set_completer(interpreter.complete)
    readline.parse_and_bind(COMPLETEKEY + ": complete")

    done = 0
    while not done:
        try:
            cmd = raw_input(prompt)
            readline.write_history_file(HISTFILE)
            if cmd in ['exit', 'quit', 'q']:
                run_command(['quit'])
                done = 1
            else:
                run_command(shlex.split(cmd))
        except EOFError:
            print()
            done = 1
        except KeyboardInterrupt:
            print()
            done = 1


def run_command(args):
    if not args:
        return

    found_command = False
    interpreter = Interpreter()
    print('Known commands:\n', '\n '.join(interpreter.commands.keys()))
    # loop (backwards) through all parameters and find the correct command
    for i in range(len(args), 0, -1):
        relative_path = '.'.join(args[:i])
        if relative_path in interpreter.commands:
            found_command = True
            break

    if not found_command:
        print('Error - Invalid command "{0}"'.format(args[0]), file=sys.stderr)
        return -1

    print('found command', relative_path)

    interpreter.execute(relative_path, args)
