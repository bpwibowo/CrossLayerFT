import os
import logging
import subprocess
import ConfigParser
import sys

fabram_dir = 'power/FabMem/spice/ram/source/'
fabram = './ram_est'
fabcam_dir = 'power/FabMem/spice/cam/source/'
fabcam = './cam_est'

def generate_ram(words, width, read_ports, write_ports):
    # change to the fabmem dir
    cwd = os.getcwd()
    os.chdir(fabram_dir)

    # run fabmem. results are stored in 'tmp.txt'
    command = [fabram]
    command += ['0', str(words), str(width), str(read_ports), str(write_ports), '60', 'tmp.txt']
    try:
        subprocess.call(command, stdout=subprocess.PIPE, stderr=subprocess.PIPE)
    except:
        logging.error('Error calling FabMem. Did you add hspice?')
        exit(0)

    # parse 'tmp.txt' for the results
    cfg_parser = ConfigParser.ConfigParser()
    cfg_parser.read('tmp.txt')
    try:
        results = dict(cfg_parser.items('results'))
    except:
        logging.error('Problem running FabMem')
        exit(0)

    os.chdir(cwd)

    return results


def generate_cam(words, width, read_ports, write_ports):
    # change to the fabmem dir
    cwd = os.getcwd()
    os.chdir(fabcam_dir)

    # run fabmem. results are stored in 'tmp.txt'
    command = [fabcam]
    command += [str(words), str(width), str(read_ports), str(write_ports), '60', 'tmp.txt']
    try:
        print >> sys.stderr, 'Calling: ' + str(' '.join(command))
        subprocess.call(command, stdout=subprocess.PIPE, stderr=subprocess.PIPE)
    except:
        logging.error('Error calling FabMem. Did you add hspice?')
        exit(0)

    # parse 'tmp.txt' for the results
    cfg_parser = ConfigParser.ConfigParser()
    cfg_parser.read('tmp.txt')
    try:
        results = dict(cfg_parser.items('results'))
    except:
        logging.error('Problem running FabMem')
        exit(0)

    os.chdir(cwd)

    return results

