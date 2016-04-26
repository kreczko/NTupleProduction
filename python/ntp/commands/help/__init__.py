"""
    help:   lists help for available commands
            Usage:
                help [command]
"""

import ntp.commands
import importlib


class Command(ntp.commands.Command):

    def __init__(self, path=__file__, doc=__doc__):
        super(Command, self).__init__(path, doc)

    def run(self, args, variables):
        from ntp.interpreter import COMMANDS

        result = ''
        for cpath, command in COMMANDS.items():
            print cpath
            result += command().help() + '\n'

        self.__text = result
        return True
#
#     def __get_command_hierarchy(self):
#         from ntp.interpreter import COMMANDS
#         hierarchy = {}
#         for cpath, command in COMMANDS.items():
#             if '.' in cpath:
