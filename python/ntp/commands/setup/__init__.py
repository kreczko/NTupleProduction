"""
    setup: sets up the workspace area with CMSSW and the dependencies
"""

from __future__ import print_function
import ntp.commands


class Command(ntp.commands.Command):

    def __init__(self):
        super(Command, self).__init__(__file__, __doc__)

    def run(self):
        print("Setting up NTP")
