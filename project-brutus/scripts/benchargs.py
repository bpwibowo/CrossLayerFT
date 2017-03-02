Benchmarks = {
    # SPEC2006 benchmarks
    "400.perlbench": {
        "ref": {
            "args": r"-I. -I./lib checkspam.pl 2500 5 25 11 150 1 1 1 1"
            }
    },
    "401.bzip2": {
        "ref": {
            "args": r"$$/data/ref/input/input.source 280"
            },
        "train": {
            "args" : r"$$/data/all/input/input.combined 80"
            }
    },
    "403.gcc": {
        "ref": {
            "args": r"$$/data/ref/input/166.i -o 166.s"
            },
        "train": {
            "args": r"$$/data/train/input/integrate.i -o integrate.s"
            }
    },
    "429.mcf": {
        "ref": {
            "args": r"$$/data/ref/input/inp.in"
            },
        "train": {
            "args": r"$$/data/train/input/inp.in"
            }
    },
    "433.milc": {
        "ref": {
            "args": r"< $$/data/ref/input/su3imp.in"
            }
    },
    "444.namd" : {
        "ref": {
            "args": r"--input $$/data/all/input/namd.input --iterations 1 --output namd.out"
            }
    },
    "445.gobmk": {
        "ref": {
            "args": r"--quiet --mode gtp < $$/data/ref/input/13x13.tst"
            },
        "train": {
            "args": r"--quiet --mode gtp < $$/data/train/input/arb.tst"
            }
    },
    "450.soplex": {
        "ref": {
            "args": r"-s1 -e -m45000 $$/data/ref/input/pds-50.mps"
            }
    },
    "456.hmmer": {
        "ref": {
            "args": r"--fixed 0 --mean 500 --num 500000 --sd 350 --seed 0 $$/data/ref/input/retro.hmm"
            },
        "train": {
            "args": r"--fixed 0 --mean 325 --num 45000 --sd 200 --seed 0 $$/data/test/input/bombesin.hmm"
            }
    },
    "458.sjeng": {
        "ref": {
            "args": r"$$/data/ref/input/ref.txt"
            },
        "train": {
            "args": r"$$/data/train/input/train.txt"
            },
        "test": {
            "args": r"$$/data/test/input/test.txt"
            }
    },
    "462.libquantum": {
        "ref": {
            "args": r"1397 8"
            },
        "train": {
            "args": r"143 25"
            }
    },
    "464.h264ref": {
        "ref": {
            "args": r" -d $$/data/ref/input/foreman_ref_encoder_baseline.cfg"
            },
        "train": {
            "args": r" -d $$/data/train/input/foreman_train_encoder_baseline.cfg"
            },
        "test": {
            "args": r" -d $$/data/test/input/foreman_test_encoder_baseline.cfg"
            }
    },
    "470.lbm": {
        "ref": {
            "args": r"3000 $$/data/ref/input/reference.dat 0 0 $$/data/ref/input/100_100_130_ldc.of"
            }
    },
    "471.omnetpp": {
        "ref": {
            "args": r"omnetpp.ini",
            "reqs": {
                    "omnetpp.ini": r"$$/data/ref/input/omnetpp.ini",
                }
            },
        "train": {
            "args": r"omnetpp.ini",
            "reqs": {
                    "omnetpp.ini": r"$$/data/train/input/omnetpp.ini",
                }
            },
        "test": {
            "args": r"omnetpp.ini",
            "reqs": {
                    "omnetpp.ini": r"$$/data/test/input/omnetpp.ini",
                }
            }
    },
    "473.astar": {
        "ref": {
            "args": r"BigLakes2048.cfg"
            },
        "train": {
            "args": r"BigLakes1024.cfg"
            }
    },
    "483.xalancbmk": {
        "ref": {
            "args": r"-v t5.xml xalanc.xsl",
            "reqs": {
                    "t5.xml": r"$$/data/ref/input/t5.xml"
                }
            },
        "train": {
            "args": r"-v allbooks.xml xalanc.xsl",
            "reqs": {
                    "allbooks.xml": r"$$/data/train/input/allbooks.xml",
                    "train.xsd": r"$$/data/train/input/train.xsd",
                    "train.lst": r"$$/data/train/input/train.lst",
                }
            }
    },

    # Miscellaneous
    "222.montecarlo": {
        "ref": {
            "args": r""
            }
    },
}

def getargs(b, data, prefix=""):
    if b not in Benchmarks: return None
    if data not in Benchmarks[b]: return None
    if "args" not in Benchmarks[b][data]: return None

    s = Benchmarks[b][data]["args"]
    return s.replace("$$",prefix+"exp/bench/CPU2006v1.0/benchspec/CPU2006/"+b)

def getreqs(b, data):
    if b not in Benchmarks: return None
    if data not in Benchmarks[b]: return None
    if "reqs" not in Benchmarks[b][data]: return None
    if len(Benchmarks[b][data]["reqs"]) == 0: return None

    reqs = {}
    for f in Benchmarks[b][data]["reqs"]:
        s = Benchmarks[b][data]["reqs"][f]
        reqs[f] = \
            s.replace("$$","exp/bench/CPU2006v1.0/benchspec/CPU2006/"+b)
    return reqs
