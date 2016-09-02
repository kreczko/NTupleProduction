import unittest
import ntp
from os.path import exists, isabs

class TestInterpreter(unittest.TestCase):

    def setUp(self):
        self.command = ntp.commands.run.analysis.Command()

    def test_input_files_from_path_wildcard(self):
        files = self.command.input_files_from_path('data/ScaleFactors/*.root')
        self.assertEqual(4, len(files))
        self.assertTrue(all([exists(f) for f in files]))
        self.assertTrue(all([isabs(f) for f in files]))

    def test_input_files_from_paths(self):
        f1 = 'data/ScaleFactors/hadronLegEfficiencies_electron.root'
        files = self.command.input_files_from_path(f1)
        self.assertEqual(1, len(files))
        self.assertTrue(all([exists(f) for f in files]))
        self.assertTrue(all([isabs(f) for f in files]))

    def test_input_files_from_path_with_comma(self):
        f1 = 'data/ScaleFactors/hadronLegEfficiencies_electron.root'
        f2 = 'data/ScaleFactors/scaleFactors_electron_id_iso.root'
        path = ','.join([f1, f2])
        files = self.command.input_files_from_path(path)
        self.assertEqual(2, len(files))
        self.assertTrue(all([exists(f) for f in files]))
        self.assertTrue(all([isabs(f) for f in files]))
        
    def test_input_files_from_nonexisting_path(self):
        f1 = 'data/ScaleFactors/hadronLegEfficiencies_electron.root'
        f2 = 'data/ScaleFactors/doesnotexist.root'
        path = ','.join([f1, f2])
        files = self.command.input_files_from_path(path)
        self.assertEqual(1, len(files))
        self.assertTrue(all([exists(f) for f in files]))
        self.assertTrue(all([isabs(f) for f in files]))
        
    def test_input_files_from_dataset(self):
        # This test requires HDFS
        if exists('/hdfs/TopQuarkGroup'):
            dataset = 'TTJets_PowhegPythia8'
            files = self.command.input_files_from_dataset(dataset)
            
        

if __name__ == '__main__':
    unittest.main()
