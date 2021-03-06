"""
    diff cutflow:
        Shows the difference between two cutflows based on their JSON files
        as created with "create cutflow <ntuple file> format=JSON". 
        The result will be a JSON file with the difference.
        
    Usage:
        diff cutflow <json 1> <json 2> [cut=<name of a cut>]
    
    Parameters:
        cut: A string representation of the cut you want to compare to.
             By default all cuts are compared.
        
"""
import logging
import json
import os
from .. import Command as C
from ntp.commands.setup import RESULTDIR

LOG = logging.getLogger(__name__)


class Command(C):

    def __init__(self, path=__file__, doc=__doc__):
        super(Command, self).__init__(path, doc)

    def run(self, args, variables):
        self.__prepare(args, variables)
        json_files = args[0:2]
        data = self.__get_content(json_files)
        diff = self.__compare(data)
        output_file = os.path.join(RESULTDIR, 'diff.json')
        with open(output_file, 'w+') as f:
            f.write(json.dumps(diff, indent=4))
        LOG.info('Created diff JSON file: {0}'.format(output_file))
        self.__print_summary(diff)
        return True

    def __get_content(self, json_files):
        data = []
        for file_name in json_files:
            with open(file_name) as f:
                d = json.load(f)
                data.append(d)
        return data

    def __compare(self, data):
        d1, d2 = data
        cut = None
        if 'cut' in self.__variables:
            cut = self.__variables['cut']

        diff = {}
        for step, d in d1.items():
            if cut and not cut == step:
                continue
            if not step in d2:
                LOG.warning(
                    'Second JSON does not have selection step "{0}"'.format(step))
                continue
            diff[step] = {}
            diff[step]['passing'] = {}
            diff[step]['summary'] = 0

            data1 = d['passing']
            data2 = d2[step]['passing']

            for run, lumis in data1.items():
                if run in data2:
                    lumis2 = data2[run]
                    diff[step]['passing'][run] = {}
                    for lumi, events in lumis.items():
                        if lumi in lumis2:
                            events2 = lumis2[lumi]
                            if events == events2:
                                continue
                            events_diff = list(set(events) ^ set(events2))
                            diff[step]['passing'][run][lumi] = events_diff

                        else:
                            diff[step]['passing'][run][lumi] = events
                else:
                    diff[step]['passing'][run] = lumis

        summary = self.__create_summary(data, diff)
        for cut in diff.keys():
            diff[cut]['summary'] = summary[cut]
        return diff

    def __create_summary(self, data, diff):
        d1, d2 = data
        cuts = d1.keys()
        summary = {}

        for cut in cuts:
            n1 = sum(
                [len(events) for lumis in d1[cut]['passing'].values() for events in lumis.values()])
            n2 = sum(
                [len(events) for lumis in d2[cut]['passing'].values() for events in lumis.values()])
            n_diff = sum(
                [len(events) for lumis in diff[cut]['passing'].values() for events in lumis.values()])
            if n1 - n2 < 0:
                summary[cut] = -1 * n_diff
            else:
                summary[cut] = n_diff

        return summary

    def __print_summary(self, diff):
        LOG.info('==================================')
        LOG.info('------Summary of differences------')
        LOG.info('==================================')
        LOG.info('Selection Step : difference (# events)')
        result = {}
        for cut in diff.keys():
            result[cut] = diff[cut]['summary']

        print(json.dumps(result, indent=4))
