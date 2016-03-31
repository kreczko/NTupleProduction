from __future__ import print_function
from collections import namedtuple
import importlib
import os
import types
import shlex
import sys

CURRENT_PATH = os.path.split(__file__)[0]
PATH_TO_NTP = os.path.join(CURRENT_PATH, '..')
BASE_MODULE_PATH = 'ntp.commands'

sys.path.append(PATH_TO_NTP)

CommandHierarchy = namedtuple('CommandHierarchy', ['isExecutable', 'children'])


def get_modules(command_path):
    '''
        Reads the folder sub-structure of ntp/commands and 
        returns all found modules that contain a Command class.

        The folder structure 
        ntp/commands/list/X
        ntp/commands/list/Y
        ntp/commands/run/X
        is mapped onto
        [
            'ntp.commands.list.X'
            'ntp.commands.list.Y'
            'ntp.commands.run.X'
        ]

    '''
    modules = []
    for p, _, _ in os.walk(command_path):
        module_path = os.path.relpath(p, command_path)
        # If it's the current directory, ignore
        if module_path == '.':
            continue

        # Convert directory structure to module path
        module_path = module_path.replace('/', '.')
        module_path = '{0}.{1}'.format(BASE_MODULE_PATH, module_path)
        try:
            mod = importlib.import_module(module_path)
            if hasattr(mod, 'Command'):
                if type(mod.Command) is types.ClassType:
                    modules.append(module_path)
        except ImportError, e:
            print('Exception', e)
            continue

    return modules


def get_command_hierarchy(modules):
    '''
        Creates a hierarchy from a list of modules
        [
            'ntp.commands.list.X'
            'ntp.commands.list.Y'
            'ntp.commands.run.X'
        ]

        will be mapped to
        hierarchy = { 
            'ntp': {
                'commands': {
                    'list': {'X': Command, 'Y': Command},
                    'run': {'X':Command}
                    }
                }
            } 
    '''
    hiera = CommandHierarchy(False, {})
    for mod in modules:
        temp = hiera
        mod_split = mod.split('.')

        n_levels = len(mod_split)

        for i, m in enumerate(mod_split):
            if not m in temp.children:
                # Set last module as a runnable command
                isExecutable = i == (n_levels - 1)
                temp.children[m] = CommandHierarchy(isExecutable, {})
            temp = temp.children[m]

    return hiera


def traverse_hierarchy(hierarchy, tokens, incomplete):
    results = []
    if not hierarchy.children:
        return results

    if len(tokens) > 1:
        t = tokens.pop(0)
        if t in hierarchy.children:
            child = hierarchy.children[t]
            results.extend(traverse_hierarchy(child, tokens, incomplete))
    elif len(tokens) == 1:
        t = tokens.pop(0)
        if t in hierarchy.children:
            child = hierarchy.children[t]
            if incomplete:
                results.append(t)
            else:
                if child.isCommand and hierarchy.children:
                    results.append('')
                results.extend(traverse_hierarchy(child, tokens, incomplete))
        else:
            for child in hierarchy.children:
                if child.startswith(t):
                    results.append(child)
    else:
        for child in hierarchy.children:
            results.append(child)


def print_hierarchy(hierarchy, indent=0):
    for key, value in hierarchy.children.items():
        print('  ' * indent + str(key))
        if isinstance(value, CommandHierarchy):
            print_hierarchy(value, indent + 1)
        else:
            print('  ' * (indent + 1) + str(value))


class Interpreter:

    def __init__(self):
        self.command_path = os.path.join(CURRENT_PATH, 'commands')
        self.modules = get_modules(self.command_path)
        self.hierarchy = get_command_hierarchy(self.modules)

    def auto_complete(self,  token, state):
        line_buffer = readline.get_line_buffer()
        incomplete = not (len(line_buffer) > 0 and line_buffer[-1] == ' ')
        tokens = line_buffer.split()
        results = []
        results = traverse_hierarchy(self.cmd_hierararchy, tokens, incomplete)
        results = [r + ' ' for r in results]

        return results[state]

    @staticmethod
    def run_command(args):
        if not args:
            return

        module = None
        rc = False
        cmd = args[0].split()

        if len(cmd) > 1:
            module_path = '{0}.{1}'.format(BASE_MODULE_PATH, '.'.join(cmd))
            try:
                module = importlib.import_module(module_path)
            except ImportError, e:
                module = None

        if module is None:
            for i in range(len(args), 0, -1):
                module_path = '{0}.{1}'.format(
                    BASE_MODULE_PATH, '.'.join(args[:i]))
                try:
                    module = importlib.import_module(module_path)
                    if module:
                        break
                except ImportError:
                    continue

        if not module:
            sys.stderr.write('Error - Invalid command "{0}"\n'.format(args[0]))
            return -1
        name = ' '.join(module_path.split('.')[2:])

        if not hasattr(module, 'Command'):
            print('HELP!')
            return -1

        try:
            command = getattr(module, 'Command')()
            rc = command.run()
        except:
            print('Command failed')

        # TODO, get any output from the command
        output = ''
        if len(output) > 0:
            print(output, end='')
            if output[len(output) - 1] != '\n':
                print()

        if rc is True:
            return 0
        return -1


if __name__ == '__main__':
    #     i = Interpreter()
    #     print_hierarchy(i.hierarchy)
    run_cli('ntp > ')
