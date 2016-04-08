from __future__ import print_function
import ntp.commands


class Command(ntp.commands.Command):

    def run(self):
        print("Running on the grid")
