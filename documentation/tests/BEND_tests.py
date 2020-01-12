from nose.tools import *
import BEND

def setup():
    print "SETUP!"
    return cmp(sorted(roots.keys()), sorted(ref.chords.keys()))

def teardown():
    print "TEAR DOWN!"

def test_basic():
    print "I RAN!"
