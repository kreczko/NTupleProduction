"""
    run local:    runs a specified job on the machine running this command
"""
from __future__ import print_function
import ntp.commands


class Command(ntp.commands.Command):

    def __init__(self, path=__file__, doc=__doc__):
        super(Command, self).__init__(path, doc)

    def run(self):
        print("Running locally")
