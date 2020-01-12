##!/usr/bin/env python
from module import ref, cls

def add_sort_notes(i):
    found = False
    for n in notes:
        if i[0] == n.name and ref.notes[i[0]] + 12 * int(i[1]) >= n.value:
            found = True
            break
        elif i[0] == n.name and ref.notes[i[0]] + 12 * int(i[1]) < notes[0].value:
            notes[notes.index(n)] = cls.Note(key.value, i, key.scale)
            found = True
            break
        else: pass
    if not found:
        newnote = cls.Note(key.value, i, key.scale)
        notes.append(newnote)
    else: pass
    return sorted(notes, key = lambda o: o.value) # sort by attribute

def cloze(s):
    '''Replace place-holders in dict items with relevant information.'''
    s = s.replace('TONIC', key.tonic)
    s = s.replace('SUPT' , key.supt)
    s = s.replace('MED'  , key.med)
    s = s.replace('SUBD' , key.subd)
    s = s.replace('DOM'  , key.dom)
    s = s.replace('SUBM' , key.subm)
    s = s.replace('SUBT' , key.subt)
    s = s.replace('INV'  , chord.inversion)
    return s

def note_info():
    '''Prints information about a note.'''
    if notes[0].deg_val == 0 or notes[0].deg_val > 1: s = 's'
    else: s = ''
    print ' NOTE '.center(30,'*')
    print '%s is %s of %s\n(%i Semi-tone%s above the tonic).' \
            % (notes[0].name, notes[0].degree, key.name,
               notes[0].deg_val, s)

def interval_info():
    '''Prints information about an interval.'''
    semitones = (notes[1].value - notes[0].value) % 12
    if semitones == 0 or semitones > 1: s = 's'
    else: s = ''
    print ' INTERVAL '.center(30,'*')
    print 'The bass note is %s and %s is\n%s (%i Semi-tone%s)' \
        % (notes[0].name, notes[1].name, chord.name, semitones, s),
    print 'above it.'

def chord_info():
    '''Prints the chord description.'''
    print ' CHORD ANALYSIS '.center(30,'*')
    print cloze(chord.name)


key = cls.Key(raw_input('\nEnter a key (tonic & mode) > ').split(' '))
notes = []

while len(notes) < 5: # Increase as more complex chords are defined.
    
    notes = add_sort_notes(raw_input('\nNote and range? > ').split(' '))
    
    if len(notes) < 2: pass
    else: chord = cls.Chord(key.mode, notes)

    if len(notes) == 1: note_info()
    elif len(notes) == 2: interval_info()
    else: chord_info()
