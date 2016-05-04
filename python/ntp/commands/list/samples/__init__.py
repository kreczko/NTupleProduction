"""
    samples:    lists all available samples

        Usage:
            list samples [campaign=<X>]
        Parameters:
            campaign: Filter output by campaign
"""
from .. import Command as C


def get_dataset_name(cfg):
    dataset = '<not found>'
    with open(cfg) as f:
        for line in f.readlines():
            if line.startswith('config.Data.inputDataset'):
                dataset = line.split('=')[-1]
                dataset = dataset.strip()
                dataset = dataset.replace("'", '')
    return dataset


def create_sample_list():
    import os
    import glob
    NTPROOT = os.environ['NTPROOT']
    CRAB_CFG_DIR = NTPROOT + '/python/crab'
    cfgs = glob.glob(CRAB_CFG_DIR + '/*/*.py')

    samples = {}
    for cfg in cfgs:
        if '__init__.py' in cfg:
            continue
        campaign, alias = cfg.split('/')[-2:]
        alias = alias.replace('.py', '')
        dataset = get_dataset_name(cfg)
        if not campaign in samples:
            samples[campaign] = {}
        samples[campaign][alias] = dataset

    return samples


def get_text_lenghts(samples):
    campaigns = samples.keys()
    aliases = []
    datasets = []

    for campaign, sample in samples.items():
        aliases.extend(sample.keys())
        datasets.extend(samples.values())

    len_campaigns = [len(c) for c in campaign]
    len_aliases = [len(a) for a in aliases]
    len_datasets = [len(d) for d in datasets]

    return max(len_campaigns), max(len_aliases), max(len_datasets)


class Command(C):

    def __init__(self, path=__file__, doc=__doc__):
        super(Command, self).__init__(path, doc)

    def run(self, args, variables):
        self.__prepare(args, variables)
        samples = create_sample_list()
        self.__create_table(samples)

        return True

    def __create_table(self, samples):
        headers = ['CAMPAIGN', 'ALIAS', 'DATASET']
        # get maximum lenghts of our columns
        max_len_c, max_len_a, max_len_d = get_text_lenghts(samples)
        # add some space
        max_len_c = max([max_len_c, len(headers[0])])

        row_format = "{:<" + str(max_len_c) + "}\t"
        row_format += "{:<" + str(max_len_a) + "}\t"
        row_format += "{:<" + str(max_len_d) + "}\n"

        self.__text = row_format.format(*headers)
        self.__text += '-' * (max_len_c + max_len_a + max_len_d)
        self.__text += '\n'

        restrict_c = ''
        if 'campaign' in self.__variables:
            restrict_c = self.__variables['campaign']
            if not restrict_c in samples:
                self.__text = 'Campaign "{}" does not exist.\n'.format(
                    restrict_c)
                self.__text += 'Available campaigns:\n'
                for c in samples.keys():
                    self.__text += '\t{}\n'.format(c)
        for campaign, sample in samples.items():
            if restrict_c:
                if not campaign == restrict_c:
                    continue
            for alias, dataset in sample.items():
                self.__text += row_format.format(*[campaign, alias, dataset])
            self.__text += '\n'
