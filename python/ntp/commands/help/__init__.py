"""
    help:   lists help for available commands
            Usage:
                help [command]
"""
from __future__ import print_function
import ntp.commands
import importlib
import ntp.interpreter
import textwrap


class Command(ntp.commands.Command):

    def __init__(self, path=__file__, doc=__doc__):
        super(Command, self).__init__(path, doc)

    def run(self, args, variables):
        from ntp.interpreter import HIERARCHY
        self.__text = self.__collect_help_text(HIERARCHY, 'COMMANDS:')

        return True

    def __collect_help_text(self, hierarchy, text, level=0):
        """
            Walks through the command hierarchy and adds help texts to 'text'.
        """
        if 'this' in hierarchy:
            help_str = hierarchy['this']().help()
            help_str = textwrap.dedent(help_str)
            for line in help_str.split('\n'):
                text += '    ' * level
                text += line
                text += '\n'
            if len(hierarchy) > 1:
                text += '    ' * level + '**SUBCOMMANDS**:\n'
                text += '        ' * level + '-' * 50
        # add subcommands
        for name, command in hierarchy.items():
            if name == 'this':
                continue
            text = self.__collect_help_text(
                command, text, level=level + 1)

        if level == 1:
            text += '    ' + '=' * 74

        return text
