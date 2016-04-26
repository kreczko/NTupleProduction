import os

class Command(object):
    """Base class for all NTP commands"""
    REQUIRE_GRID_CERT = False
    
    def __init__(self, path = __file__, doc = __doc__):
        current_path = os.path.split(path)[0]
        parent_folder = current_path.split("/")[-1]
        if parent_folder == 'commands':
            self.__name = 'ntp'
        else:
            self.__name = parent_folder
        self.__doc = doc
        self.__text = ''
    
    def _execute(self):
        pass
    
    def run(self, args, variables):
        """
            Runs the commands and an exit code (True if everything went OK)
        """
        pass
    
    def help(self):
        """Returns a little help text with the description of the command."""
        if self.__doc:
            return self.__doc
        else:
            return 'Documentation for command "{0}" is missing'.format(self.__name)
    
    def parse_arguments(self, name, argv):
        pass
    
    def get_text(self):
        return self.__text
        