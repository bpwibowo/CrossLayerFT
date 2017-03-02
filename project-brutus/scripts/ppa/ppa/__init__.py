import argparse
import logging
import yaml

from ppa import yaml_ordered_dict
from ppa import processing
from ppa import power_db

def run_app():

    # add/process input arguments
    argparser = argparse.ArgumentParser()
    argparser.add_argument('-p', '--perfect_pipelining', 
                           dest='perfect_pipelining',
                           help='Assume all stages (fetch to execute) are perfectly balanced',
                           action='store_true')
    argparser.add_argument('-v',
                           dest='verbosity',
                           help='Increase the level of detail printed. (max: -vv)',
                           action='count')
    argparser.add_argument('--drop_table',
                           help='drop a table from power.db (see ppa/power_db.py for class/table names)')
    argparser.add_argument('--initialize_db',
                           action='store_true',
                           help='populate a new db with data from power/')
    argparser.add_argument('--debug', 
                           dest='loglevel',
                           action='store_const',
                           const=logging.DEBUG,
                           default=logging.INFO,
                           help='print debug messages')
    argparser.add_argument('-i', '--yamlfile', 
                           required=True,
                           help='PPA file in YAML format to process')
    args = argparser.parse_args()

    # create the logger
    log_format = '[%(levelname)s] %(filename)s::%(funcName)s:%(lineno)d: %(message)s'
    logging.basicConfig(level=args.loglevel, format=log_format)

    # remove a table from the db, causing it to be repopulated
    if args.drop_table:
        power_db.drop_table(args.drop_table)

    # initialize a new db
    if args.initialize_db:
        power_db.initialize()

    # process the yaml file into dicts/lists
    logging.info('Processing ' + args.yamlfile)
    desc = yaml.load(open(args.yamlfile), yaml_ordered_dict.OrderedDictYAMLLoader)

    # start processing the processor. everything descends from here
    processing.process(info=desc['processor'], perfect_pipelining=args.perfect_pipelining)

    processing.dump(verbosity=2)

