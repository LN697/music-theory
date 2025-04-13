#include "common.hpp"

class Note {
    private:
        std::string name;
        int midiValue; // MIDI value for the note (C4 = 60)

    public:
        Note() : name(""), midiValue(0) {}
        Note(const std::string& noteName, int value) : name(noteName), midiValue(value) {}
        
        std::string getName() const { return name; }
        int getMidiValue() const { return midiValue; }
        
        static const std::vector<std::string> ALL_NOTES;
        
        // Returns a note that is a specified number of semitones above this note
        Note transpose(int semitones) const {
            int newMidiValue = midiValue + semitones;
            int noteIndex = (newMidiValue % 12);
            std::string newName = ALL_NOTES[noteIndex];
            return Note(newName, newMidiValue);
        }
};

const std::vector<std::string> Note::ALL_NOTES = {"C", "C#/Db", "D", "D#/Eb", "E", "F", "F#/Gb", "G", "G#/Ab", "A", "A#/Bb", "B"};

class Scale {
    private:
        std::string name;
        std::vector<int> intervals; // Intervals in semitones
        Note rootNote;

    public:
        Scale(const std::string& scaleName, const std::vector<int>& scaleIntervals, const Note& root)
            : name(scaleName), intervals(scaleIntervals), rootNote(root) {}
        
        std::string getName() const { return name; }
        
        std::vector<Note> getNotes() const {
            std::vector<Note> notes;
            notes.push_back(rootNote);
            
            int currentPosition = rootNote.getMidiValue();
            for (size_t i = 0; i < intervals.size(); ++i) {
                currentPosition += intervals[i];
                notes.push_back(Note(Note::ALL_NOTES[currentPosition % 12], currentPosition));
            }
            
            return notes;
        }
        
        void print() const {
            std::cout << name << " Scale (" << rootNote.getName() << "): ";
            auto notes = getNotes();
            for (const auto& note : notes) {
                std::cout << note.getName() << " ";
            }
            std::cout << std::endl;
        }
        
        static Scale majorScale(const Note& root) {
            return Scale(root.getName() + " Major", {2, 2, 1, 2, 2, 2, 1}, root);
        }
        
        static Scale minorScale(const Note& root) {
            return Scale(root.getName() + " Minor", {2, 1, 2, 2, 1, 2, 2}, root);
        }
        
        static Scale pentatonicMajor(const Note& root) {
            return Scale(root.getName() + " Pentatonic Major", {2, 2, 3, 2, 3}, root);
        }
        
        static Scale pentatonicMinor(const Note& root) {
            return Scale(root.getName() + " Pentatonic Minor", {3, 2, 2, 3, 2}, root);
        }
        
        static Scale bluesScale(const Note& root) {
            return Scale(root.getName() + " Blues", {3, 2, 1, 1, 3, 2}, root);
        }
};

class Chord {
    private:
        std::string name;
        std::vector<int> intervals; // Intervals in semitones from the root
        Note rootNote;

    public:
        Chord(const std::string& chordName, const std::vector<int>& chordIntervals, const Note& root)
            : name(chordName), intervals(chordIntervals), rootNote(root) {}
        
        std::string getName() const { return name; }
        
        std::vector<Note> getNotes() const {
            std::vector<Note> notes;
            notes.push_back(rootNote);
            
            // int currentPosition = rootNote.getMidiValue();
            for (auto interval : intervals) {
                notes.push_back(rootNote.transpose(interval));
            }
            
            return notes;
        }
        
        void print() const {
            std::cout << name << " Chord: ";
            auto notes = getNotes();
            for (const auto& note : notes) {
                std::cout << note.getName() << " ";
            }
            std::cout << std::endl;
        }
        
        static Chord major(const Note& root) {
            return Chord(root.getName() + " Major", {4, 7}, root);
        }
        
        static Chord minor(const Note& root) {
            return Chord(root.getName() + " Minor", {3, 7}, root);
        }
        
        static Chord dominant7(const Note& root) {
            return Chord(root.getName() + "7", {4, 7, 10}, root);
        }
        
        static Chord major7(const Note& root) {
            return Chord(root.getName() + "Maj7", {4, 7, 11}, root);
        }
        
        static Chord minor7(const Note& root) {
            return Chord(root.getName() + "min7", {3, 7, 10}, root);
        }
};

class ChordProgression {
    private:
        std::string name;
        std::vector<Chord> chords;

    public:
        ChordProgression(const std::string& progressionName, const std::vector<Chord>& progressionChords)
            : name(progressionName), chords(progressionChords) {}
        
        void print() const {
            std::cout << name << " Progression:" << std::endl;
            for (size_t i = 0; i < chords.size(); ++i) {
                std::cout << "  " << (i+1) << ". " << chords[i].getName() << std::endl;
            }
        }
        
        static ChordProgression createFromRomanNumerals(const Scale& scale, const std::vector<std::string>& numerals, const std::string& name) {
            std::vector<Chord> progressionChords;
            auto scaleNotes = scale.getNotes();
            
            for (const auto& numeral : numerals) {
                int degree = 0;
                Chord chord(numeral, {}, scaleNotes[0]);
                
                if (numeral == "I" || numeral == "i") degree = 0;
                else if (numeral == "II" || numeral == "ii") degree = 1;
                else if (numeral == "III" || numeral == "iii") degree = 2;
                else if (numeral == "IV" || numeral == "iv") degree = 3;
                else if (numeral == "V" || numeral == "v") degree = 4;
                else if (numeral == "VI" || numeral == "vi") degree = 5;
                else if (numeral == "VII" || numeral == "vii") degree = 6;
                
                if (numeral.find('7') != std::string::npos) {
                    if (std::isupper(numeral[0])) {
                        chord = Chord::dominant7(scaleNotes[degree]);
                    } else {
                        chord = Chord::minor7(scaleNotes[degree]);
                    }
                } else {
                    if (std::isupper(numeral[0])) {
                        chord = Chord::major(scaleNotes[degree]);
                    } else {
                        chord = Chord::minor(scaleNotes[degree]);
                    }
                }
                
                progressionChords.push_back(chord);
            }
            
            return ChordProgression(name, progressionChords);
        }
};


// TODO: change to private, i.e., implement getters/setters for fretboard
class GuitarFretboard {
    public:
        std::vector<std::string> standardTuning;
        std::vector<std::vector<Note>> fretboard;
        int numStrings;
        int numFrets;

    public:
        const std::vector<std::vector<Note>>& getFretboard() const {
            return fretboard;
        }

    public:
        GuitarFretboard(int frets = 24) : numStrings(6), numFrets(frets) {
            // Standard tuning: E A D G B E
            standardTuning = {"E", "A", "D", "G", "B", "E"};
            
            // Initialize the fretboard
            initializeFretboard();
        }
        
        void initializeFretboard() {
            fretboard.resize(numStrings);
            
            // MIDI values for open strings in standard tuning (high to low)
            std::vector<int> openStringMidi = {64, 59, 55, 50, 45, 40};
            
            for (int string = 0; string < numStrings; ++string) {
                fretboard[string].resize(numFrets + 1); // +1 for the open string
                
                for (int fret = 0; fret <= numFrets; ++fret) {
                    int midiValue = openStringMidi[string] + fret;
                    int noteIndex = midiValue % 12;
                    fretboard[string][fret] = Note(Note::ALL_NOTES[noteIndex], midiValue);
                }
            }
        }
        
        void printFretboard(int startFret = 0, int endFret = 12) const {
            // Print fret numbers
            std::cout << "    ";
            for (int fret = startFret; fret <= endFret; ++fret) {
                std::cout << std::setw(5) << fret;
            }
            std::cout << std::endl;
            
            // Print a line
            std::cout << "    ";
            for (int fret = startFret; fret <= endFret; ++fret) {
                std::cout << "-----";
            }
            std::cout << std::endl;
            
            // Print each string
            for (int string = 0; string < numStrings; ++string) {
                std::cout << standardTuning[string] << " | ";
                for (int fret = startFret; fret <= endFret; ++fret) {
                    std::cout << std::setw(5) << fretboard[string][fret].getName();
                }
                std::cout << std::endl;
            }
        }
        
        void highlightScale(const Scale& scale) const {
            auto scaleNotes = scale.getNotes();
            std::vector<std::string> noteNames;
            
            for (const auto& note : scaleNotes) {
                noteNames.push_back(note.getName());
            }
            
            // Print fret numbers
            std::cout << "    ";
            for (int fret = 0; fret <= 12; ++fret) {
                std::cout << std::setw(5) << fret;
            }
            std::cout << std::endl;
            
            // Print a line
            std::cout << "    ";
            for (int fret = 0; fret <= 12; ++fret) {
                std::cout << "-----";
            }
            std::cout << std::endl;
            
            // Print each string with scale notes highlighted
            for (int string = 0; string < numStrings; ++string) {
                std::cout << standardTuning[string] << " | ";
                for (int fret = 0; fret <= 12; ++fret) {
                    std::string noteName = fretboard[string][fret].getName();
                    bool isInScale = false;
                    
                    for (const auto& scaleName : noteNames) {
                        if (noteName == scaleName || 
                            (noteName.find(scaleName) != std::string::npos) ||
                            (scaleName.find(noteName) != std::string::npos)) {
                            isInScale = true;
                            break;
                        }
                    }
                    
                    if (isInScale) {
                        std::cout << std::setw(5) << "[" + noteName + "]";
                    } else {
                        std::cout << std::setw(5) << ".";
                    }
                }
                std::cout << std::endl;
            }
        }
        
        void highlightChord(const Chord& chord) const {
            auto chordNotes = chord.getNotes();
            std::vector<std::string> noteNames;
            
            for (const auto& note : chordNotes) {
                noteNames.push_back(note.getName());
            }
            
            // Print fret numbers
            std::cout << "    ";
            for (int fret = 0; fret <= 12; ++fret) {
                std::cout << std::setw(5) << fret;
            }
            std::cout << std::endl;
            
            // Print a line
            std::cout << "    ";
            for (int fret = 0; fret <= 12; ++fret) {
                std::cout << "-----";
            }
            std::cout << std::endl;
            
            // Print each string with chord notes highlighted
            for (int string = 0; string < numStrings; ++string) {
                std::cout << standardTuning[string] << " | ";
                for (int fret = 0; fret <= 12; ++fret) {
                    std::string noteName = fretboard[string][fret].getName();
                    bool isInChord = false;
                    
                    for (const auto& chordName : noteNames) {
                        if (noteName == chordName || 
                            (noteName.find(chordName) != std::string::npos) ||
                            (chordName.find(noteName) != std::string::npos)) {
                            isInChord = true;
                            break;
                        }
                    }
                    
                    if (isInChord) {
                        std::cout << std::setw(5) << "[" + noteName + "]";
                    } else {
                        std::cout << std::setw(5) << ".";
                    }
                }
                std::cout << std::endl;
            }
        }
};

class IntervalTrainer {
    private:
        std::map<std::string, int> intervalMap = {
            {"Minor 2nd", 1}, {"Major 2nd", 2}, {"Minor 3rd", 3}, {"Major 3rd", 4},
            {"Perfect 4th", 5}, {"Tritone", 6}, {"Perfect 5th", 7}, {"Minor 6th", 8},
            {"Major 6th", 9}, {"Minor 7th", 10}, {"Major 7th", 11}, {"Octave", 12}
        };

    public:
        void printIntervalDefinitions() const {
            std::cout << "Common Intervals:" << std::endl;
            for (const auto& interval : intervalMap) {
                std::cout << std::setw(12) << interval.first << ": " << interval.second << " semitones" << std::endl;
            }
        }
        
        std::string identifyInterval(const Note& note1, const Note& note2) const {
            int semitones = std::abs(note2.getMidiValue() - note1.getMidiValue()) % 12;
            
            for (const auto& interval : intervalMap) {
                if (interval.second == semitones) {
                    return interval.first;
                }
            }
            
            return "Unknown interval";
        }
        
        void practiceIntervals() const {
            std::cout << "Interval Training Exercise:" << std::endl;
            std::cout << "For each pair of notes, identify the interval." << std::endl;
            
            // Create a random seed
            std::srand(std::time(nullptr));
            
            for (int i = 1; i <= 5; ++i) {
                // Generate random starting note
                int noteIndex = std::rand() % 12;
                Note startNote(Note::ALL_NOTES[noteIndex], 60 + noteIndex);
                
                // Generate random interval (1-12 semitones)
                int intervalSize = 1 + std::rand() % 12;
                Note endNote = startNote.transpose(intervalSize);
                
                std::cout << "Exercise " << i << ": " << startNote.getName() << " to " << endNote.getName();
                std::cout << " (Press Enter to see answer)";
                std::cin.ignore();
                
                std::string intervalName = "Unknown";
                for (const auto& interval : intervalMap) {
                    if (interval.second == intervalSize) {
                        intervalName = interval.first;
                        break;
                    }
                }
                
                std::cout << "Answer: " << intervalName << " (" << intervalSize << " semitones)" << std::endl;
            }
        }
};

class EarTrainer {
    private:
        std::vector<std::string> chordQualities = {
            "Major", "Minor", "Dominant 7", "Major 7", "Minor 7"
        };

    public:
        void practiceChordRecognition() const {
            std::cout << "Chord Recognition Exercise:" << std::endl;
            std::cout << "For each question, identify the chord quality." << std::endl;
            
            // Create a random seed
            std::srand(std::time(nullptr));
            
            for (int i = 1; i <= 5; ++i) {
                // Generate random root note
                int noteIndex = std::rand() % 12;
                Note rootNote(Note::ALL_NOTES[noteIndex], 60 + noteIndex);
                
                // Generate random chord quality
                int qualityIndex = std::rand() % chordQualities.size();
                std::string quality = chordQualities[qualityIndex];
                
                Chord chord("", {}, rootNote);
                if (quality == "Major") {
                    chord = Chord::major(rootNote);
                } else if (quality == "Minor") {
                    chord = Chord::minor(rootNote);
                } else if (quality == "Dominant 7") {
                    chord = Chord::dominant7(rootNote);
                } else if (quality == "Major 7") {
                    chord = Chord::major7(rootNote);
                } else if (quality == "Minor 7") {
                    chord = Chord::minor7(rootNote);
                }
                
                std::cout << "Exercise " << i << ": Identify the quality of this chord: ";
                
                // Print chord notes
                auto notes = chord.getNotes();
                for (const auto& note : notes) {
                    std::cout << note.getName() << " ";
                }
                
                std::cout << "(Press Enter to see answer)";
                std::cin.ignore();
                
                std::cout << "Answer: " << rootNote.getName() << " " << quality << std::endl;
            }
        }
};

class MusicTheoryCompanion {
    private:
        GuitarFretboard fretboard;
        IntervalTrainer intervalTrainer;
        EarTrainer earTrainer;

    public:
        MusicTheoryCompanion() : fretboard(24) {}
        
        void showMainMenu() {
            int choice = 0;
            
            while (choice != 9) {
                std::cout << "\n=== Guitar Music Theory Companion ===" << std::endl;
                std::cout << "1. View Guitar Fretboard" << std::endl;
                std::cout << "2. Explore Scales" << std::endl;
                std::cout << "3. Explore Chords" << std::endl;
                std::cout << "4. Chord Progressions" << std::endl;
                std::cout << "5. Interval Training" << std::endl;
                std::cout << "6. Ear Training" << std::endl;
                std::cout << "7. Music Theory Concepts" << std::endl;
                std::cout << "8. Practice Exercises" << std::endl;
                std::cout << "9. Quit" << std::endl;
                std::cout << "Enter your choice: ";
                std::cin >> choice;
                
                switch (choice) {
                    case 1:
                        showFretboardMenu();
                        break;
                    case 2:
                        showScalesMenu();
                        break;
                    case 3:
                        showChordsMenu();
                        break;
                    case 4:
                        showProgressionsMenu();
                        break;
                    case 5:
                        showIntervalTrainingMenu();
                        break;
                    case 6:
                        showEarTrainingMenu();
                        break;
                    case 7:
                        showMusicTheoryConceptsMenu();
                        break;
                    case 8:
                        showPracticeExercisesMenu();
                        break;
                    case 9:
                        std::cout << "Thank you for using the Guitar Music Theory Companion!" << std::endl;
                        break;
                    default:
                        std::cout << "Invalid choice. Please try again." << std::endl;
                }
            }
        }
        
        void showFretboardMenu() {
            int choice = 0;
            
            while (choice != 3) {
                std::cout << "\n=== Fretboard Visualization ===" << std::endl;
                std::cout << "1. View Complete Fretboard (0-12)" << std::endl;
                std::cout << "2. View Extended Fretboard (12-24)" << std::endl;
                std::cout << "3. Back to Main Menu" << std::endl;
                std::cout << "Enter your choice: ";
                std::cin >> choice;
                
                switch (choice) {
                    case 1:
                        fretboard.printFretboard(0, 12);
                        break;
                    case 2:
                        fretboard.printFretboard(12, 24);
                        break;
                    case 3:
                        break;
                    default:
                        std::cout << "Invalid choice. Please try again." << std::endl;
                }
            }
        }
        
        void showScalesMenu() {
            int choice = 0;
            
            while (choice != 6) {
                std::cout << "\n=== Scales Explorer ===" << std::endl;
                std::cout << "1. Major Scales" << std::endl;
                std::cout << "2. Minor Scales" << std::endl;
                std::cout << "3. Pentatonic Major Scales" << std::endl;
                std::cout << "4. Pentatonic Minor Scales" << std::endl;
                std::cout << "5. Blues Scales" << std::endl;
                std::cout << "6. Back to Main Menu" << std::endl;
                std::cout << "Enter your choice: ";
                std::cin >> choice;
                
                if (choice >= 1 && choice <= 5) {
                    std::string rootNote;
                    std::cout << "Enter root note (e.g., C, F#, Bb): ";
                    std::cin >> rootNote;
                    
                    // Find the MIDI value for the root note
                    int rootIndex = -1;
                    for (size_t i = 0; i < Note::ALL_NOTES.size(); ++i) {
                        if (Note::ALL_NOTES[i].find(rootNote) != std::string::npos) {
                            rootIndex = i;
                            break;
                        }
                    }
                    
                    if (rootIndex != -1) {
                        Note root(Note::ALL_NOTES[rootIndex], 60 + rootIndex);
                        Scale scale("", {}, root);
                        
                        switch (choice) {
                            case 1:
                                scale = Scale::majorScale(root);
                                break;
                            case 2:
                                scale = Scale::minorScale(root);
                                break;
                            case 3:
                                scale = Scale::pentatonicMajor(root);
                                break;
                            case 4:
                                scale = Scale::pentatonicMinor(root);
                                break;
                            case 5:
                                scale = Scale::bluesScale(root);
                                break;
                        }
                        
                        scale.print();
                        std::cout << "\nScale positions on fretboard:" << std::endl;
                        fretboard.highlightScale(scale);
                    } else {
                        std::cout << "Invalid root note. Please try again." << std::endl;
                    }
                }
            }
        }
        
        void showChordsMenu() {
            int choice = 0;
            
            while (choice != 6) {
                std::cout << "\n=== Chords Explorer ===" << std::endl;
                std::cout << "1. Major Chords" << std::endl;
                std::cout << "2. Minor Chords" << std::endl;
                std::cout << "3. Dominant 7th Chords" << std::endl;
                std::cout << "4. Major 7th Chords" << std::endl;
                std::cout << "5. Minor 7th Chords" << std::endl;
                std::cout << "6. Back to Main Menu" << std::endl;
                std::cout << "Enter your choice: ";
                std::cin >> choice;
                
                if (choice >= 1 && choice <= 5) {
                    std::string rootNote;
                    std::cout << "Enter root note (e.g., C, F#, Bb): ";
                    std::cin >> rootNote;
                    
                    // Find the MIDI value for the root note
                    int rootIndex = -1;
                    for (size_t i = 0; i < Note::ALL_NOTES.size(); ++i) {
                        if (Note::ALL_NOTES[i].find(rootNote) != std::string::npos) {
                            rootIndex = i;
                            break;
                        }
                    }
                    
                    if (rootIndex != -1) {
                        Note root(Note::ALL_NOTES[rootIndex], 60 + rootIndex);
                        Chord chord("", {}, root);
                        
                        switch (choice) {
                            case 1:
                                chord = Chord::major(root);
                                break;
                            case 2:
                                chord = Chord::minor(root);
                                break;
                            case 3:
                                chord = Chord::dominant7(root);
                                break;
                            case 4:
                                chord = Chord::major7(root);
                                break;
                            case 5:
                                chord = Chord::minor7(root);
                                break;
                        }
                        
                        chord.print();
                        std::cout << "\nChord positions on fretboard:" << std::endl;
                        fretboard.highlightChord(chord);
                    } else {
                        std::cout << "Invalid root note. Please try again." << std::endl;
                    }
                }
            }
        }
        
        void showProgressionsMenu() {
            int choice = 0;
            
            while (choice != 5) {
                std::cout << "\n=== Chord Progressions ===" << std::endl;
                std::cout << "1. I-IV-V (Major)" << std::endl;
                std::cout << "2. I-V-vi-IV (Pop)" << std::endl;
                std::cout << "3. ii-V-I (Jazz)" << std::endl;
                std::cout << "4. i-iv-v (Minor)" << std::endl;
                std::cout << "5. Back to Main Menu" << std::endl;
                std::cout << "Enter your choice: ";
                std::cin >> choice;
                
                if (choice >= 1 && choice <= 4) {
                    std::string rootNote;
                    std::cout << "Enter key (e.g., C, F#, Bb): ";
                    std::cin >> rootNote;
                    
                    // Find the MIDI value for the root note
                    int rootIndex = -1;
                    for (size_t i = 0; i < Note::ALL_NOTES.size(); ++i) {
                        if (Note::ALL_NOTES[i].find(rootNote) != std::string::npos) {
                            rootIndex = i;
                            break;
                        }
                    }
                    
                    if (rootIndex != -1) {
                        Note root(Note::ALL_NOTES[rootIndex], 60 + rootIndex);
                        
                        switch (choice) {
                            case 1: {
                                Scale majorScale = Scale::majorScale(root);
                                ChordProgression progression = ChordProgression::createFromRomanNumerals(
                                    majorScale, {"I", "IV", "V"}, root.getName() + " Major I-IV-V");
                                progression.print();
                                break;
                            }
                            case 2: {
                                Scale majorScale = Scale::majorScale(root);
                                ChordProgression progression = ChordProgression::createFromRomanNumerals(
                                    majorScale, {"I", "V", "vi", "IV"}, root.getName() + " Major I-V-vi-IV (Pop)");
                                progression.print();
                                break;
                            }
                            case 3: {
                                Scale majorScale = Scale::majorScale(root);
                                ChordProgression progression = ChordProgression::createFromRomanNumerals(
                                    majorScale, {"ii", "V", "I"}, root.getName() + " Major ii-V-I (Jazz)");
                                progression.print();
                                break;
                            }
                            case 4: {
                                Scale minorScale = Scale::minorScale(root);
                                ChordProgression progression = ChordProgression::createFromRomanNumerals(
                                    minorScale, {"i", "iv", "v"}, root.getName() + " Minor i-iv-v");
                                progression.print();
                                break;
                            }
                        }
                    } else {
                        std::cout << "Invalid key. Please try again." << std::endl;
                    }
                }
            }
        }
        
        void showIntervalTrainingMenu() {
            int choice = 0;
            
            while (choice != 3) {
                std::cout << "\n=== Interval Training ===" << std::endl;
                std::cout << "1. View Interval Definitions" << std::endl;
                std::cout << "2. Practice Interval Recognition" << std::endl;
                std::cout << "3. Back to Main Menu" << std::endl;
                std::cout << "Enter your choice: ";
                std::cin >> choice;
                
                switch (choice) {
                    case 1:
                        intervalTrainer.printIntervalDefinitions();
                        break;
                    case 2:
                        intervalTrainer.practiceIntervals();
                        break;
                    case 3:
                        break;
                    default:
                        std::cout << "Invalid choice. Please try again." << std::endl;
                }
            }
        }
        
        void showEarTrainingMenu() {
            int choice = 0;
            
            while (choice != 2) {
                std::cout << "\n=== Ear Training ===" << std::endl;
                std::cout << "1. Practice Chord Recognition" << std::endl;
                std::cout << "2. Back to Main Menu" << std::endl;
                std::cout << "Enter your choice: ";
                std::cin >> choice;
                
                switch (choice) {
                    case 1:
                        earTrainer.practiceChordRecognition();
                        break;
                    case 2:
                        break;
                    default:
                        std::cout << "Invalid choice. Please try again." << std::endl;
                }
            }
        }
        
        void showMusicTheoryConceptsMenu() {
            int choice = 0;
            
            while (choice != 6) {
                std::cout << "\n=== Music Theory Concepts ===" << std::endl;
                std::cout << "1. The Circle of Fifths" << std::endl;
                std::cout << "2. Diatonic Harmony" << std::endl;
                std::cout << "3. Modes of the Major Scale" << std::endl;
                std::cout << "4. Modulation Techniques" << std::endl;
                std::cout << "5. Advanced Chord Extensions" << std::endl;
                std::cout << "6. Back to Main Menu" << std::endl;
                std::cout << "Enter your choice: ";
                std::cin >> choice;
                
                switch (choice) {
                    case 1:
                        explainCircleOfFifths();
                        break;
                    case 2:
                        explainDiatonicHarmony();
                        break;
                    case 3:
                        explainModes();
                        break;
                    case 4:
                        explainModulation();
                        break;
                    case 5:
                        explainChordExtensions();
                        break;
                    case 6:
                        break;
                    default:
                        std::cout << "Invalid choice. Please try again." << std::endl;
                }
            }
        }
        
        void explainCircleOfFifths() const {
            std::cout << "\n=== The Circle of Fifths ===" << std::endl;
            std::cout << "The Circle of Fifths is a fundamental concept in music theory that shows the relationship\n"
                    << "between the 12 tones of the chromatic scale and their corresponding key signatures.\n\n"
                    << "Moving clockwise around the circle, each note is a perfect fifth above the previous:\n"
                    << "C → G → D → A → E → B → F# → C# → G# → D# → A# → F → C\n\n"
                    << "Moving counterclockwise, each note is a perfect fourth above the previous (or a fifth below).\n\n"
                    << "Key signatures follow this pattern:\n"
                    << "- C major has no sharps or flats\n"
                    << "- Moving clockwise adds one sharp each time (G has 1 sharp, D has 2 sharps, etc.)\n"
                    << "- Moving counterclockwise adds one flat each time (F has 1 flat, Bb has 2 flats, etc.)\n\n"
                    << "The circle also shows relative major/minor relationships. Each major key shares\n"
                    << "a key signature with its relative minor, which is located three semitones below.\n"
                    << "For example, C major and A minor both have no sharps or flats.\n";
        }
        
        void explainDiatonicHarmony() const {
            std::cout << "\n=== Diatonic Harmony ===" << std::endl;
            std::cout << "Diatonic harmony refers to chords that are built using only the notes from a particular scale.\n\n"
                    << "In the major scale, the diatonic chords are:\n"
                    << "I   - Major chord (built on the 1st scale degree)\n"
                    << "ii  - Minor chord (built on the 2nd scale degree)\n"
                    << "iii - Minor chord (built on the 3rd scale degree)\n"
                    << "IV  - Major chord (built on the 4th scale degree)\n"
                    << "V   - Major chord (built on the 5th scale degree)\n"
                    << "vi  - Minor chord (built on the 6th scale degree)\n"
                    << "vii°- Diminished chord (built on the 7th scale degree)\n\n"
                    << "In the natural minor scale, the diatonic chords are:\n"
                    << "i   - Minor chord\n"
                    << "ii° - Diminished chord\n"
                    << "III - Major chord\n"
                    << "iv  - Minor chord\n"
                    << "v   - Minor chord (or V - Major in harmonic minor)\n"
                    << "VI  - Major chord\n"
                    << "VII - Major chord\n\n"
                    << "These chords form the basis of chord progressions in Western music.\n";
        }
        
        void explainModes() const {
            std::cout << "\n=== Modes of the Major Scale ===" << std::endl;
            std::cout << "Modes are scales derived from the major scale by starting on different scale degrees.\n\n"
                    << "The seven modes of the C major scale are:\n"
                    << "1. Ionian (C D E F G A B C) - The major scale itself\n"
                    << "2. Dorian (D E F G A B C D) - Minor scale with raised 6th\n"
                    << "3. Phrygian (E F G A B C D E) - Minor scale with lowered 2nd\n"
                    << "4. Lydian (F G A B C D E F) - Major scale with raised 4th\n"
                    << "5. Mixolydian (G A B C D E F G) - Major scale with lowered 7th\n"
                    << "6. Aeolian (A B C D E F G A) - The natural minor scale\n"
                    << "7. Locrian (B C D E F G A B) - Diminished scale\n\n"
                    << "Each mode has a distinct character and sound:\n"
                    << "- Ionian: bright, happy, stable\n"
                    << "- Dorian: minor but with a jazzy/bluesy character\n"
                    << "- Phrygian: exotic, Spanish flavor\n"
                    << "- Lydian: dreamy, floating quality\n"
                    << "- Mixolydian: bluesy, dominant feel\n"
                    << "- Aeolian: sad, melancholic\n"
                    << "- Locrian: unstable, dissonant\n";
        }
        
        void explainModulation() const {
            std::cout << "\n=== Modulation Techniques ===" << std::endl;
            std::cout << "Modulation is the process of changing from one key to another within a piece of music.\n\n"
                    << "Common modulation techniques include:\n\n"
                    << "1. Direct Modulation (Phrase Modulation)\n"
                    << "   Simply changing to the new key without preparation.\n\n"
                    << "2. Common Chord Modulation (Pivot Chord)\n"
                    << "   Using a chord that exists in both the original and target key to smooth the transition.\n"
                    << "   Example: C major to G major using the G major chord (V in C, I in G).\n\n"
                    << "3. Chromatic Modulation\n"
                    << "   Using chromatic (non-diatonic) chords to lead to the new key.\n\n"
                    << "4. Circle of Fifths Modulation\n"
                    << "   Moving to a key that's a fifth away (very common in classical and pop music).\n\n"
                    << "5. Parallel Key Modulation\n"
                    << "   Changing from major to minor (or vice versa) while keeping the same tonic.\n"
                    << "   Example: C major to C minor.\n\n"
                    << "6. Secondary Dominant\n"
                    << "   Using the dominant chord of a non-tonic chord to temporarily emphasize that chord.\n"
                    << "   Example: In C major, using D7 (V of G) before G to briefly emphasize G.\n";
        }
        
        void explainChordExtensions() const {
            std::cout << "\n=== Advanced Chord Extensions ===" << std::endl;
            std::cout << "Chord extensions add notes beyond the basic triad structure to create richer harmonies.\n\n"
                    << "Common extensions include:\n\n"
                    << "7th Chords (add the 7th scale degree):\n"
                    << "- Major 7th (Maj7): 1-3-5-7    Example: Cmaj7 (C-E-G-B)\n"
                    << "- Dominant 7th (7): 1-3-5-b7   Example: C7 (C-E-G-Bb)\n"
                    << "- Minor 7th (min7): 1-b3-5-b7  Example: Cm7 (C-Eb-G-Bb)\n"
                    << "- Half-diminished (m7b5): 1-b3-b5-b7  Example: Cm7b5 (C-Eb-Gb-Bb)\n"
                    << "- Diminished 7th (dim7): 1-b3-b5-bb7  Example: Cdim7 (C-Eb-Gb-A)\n\n"
                    << "9th Chords (add the 9th scale degree):\n"
                    << "- Major 9th (Maj9): 1-3-5-7-9    Example: Cmaj9 (C-E-G-B-D)\n"
                    << "- Dominant 9th (9): 1-3-5-b7-9   Example: C9 (C-E-G-Bb-D)\n"
                    << "- Minor 9th (min9): 1-b3-5-b7-9  Example: Cm9 (C-Eb-G-Bb-D)\n\n"
                    << "11th and 13th chords add even more extensions:\n"
                    << "- 11th adds the 4th/11th: 1-3-5-b7-9-11\n"
                    << "- 13th adds the 6th/13th: 1-3-5-b7-9-11-13\n\n"
                    << "Altered extensions modify these notes:\n"
                    << "- b9 (flat 9): lowers the 9th by a half step\n"
                    << "- #9 (sharp 9): raises the 9th by a half step\n"
                    << "- #11 (sharp 11): raises the 11th by a half step\n"
                    << "- b13 (flat 13): lowers the 13th by a half step\n\n"
                    << "These extensions are commonly used in jazz, fusion, and progressive styles.\n";
        }
        
        void showPracticeExercisesMenu() {
            int choice = 0;
            
            while (choice != 6) {
                std::cout << "\n=== Practice Exercises ===" << std::endl;
                std::cout << "1. Fretboard Note Recognition" << std::endl;
                std::cout << "2. Scale Pattern Exercises" << std::endl;
                std::cout << "3. Chord Construction Challenge" << std::endl;
                std::cout << "4. Interval Ear Training" << std::endl;
                std::cout << "5. Common Chord Progression Practice" << std::endl;
                std::cout << "6. Back to Main Menu" << std::endl;
                std::cout << "Enter your choice: ";
                std::cin >> choice;
                
                switch (choice) {
                    case 1:
                        practiceFretboardRecognition();
                        break;
                    case 2:
                        practiceScalePatterns();
                        break;
                    case 3:
                        practiceChordConstruction();
                        break;
                    case 4:
                        intervalTrainer.practiceIntervals();
                        break;
                    case 5:
                        practiceChordProgressions();
                        break;
                    case 6:
                        break;
                    default:
                        std::cout << "Invalid choice. Please try again." << std::endl;
                }
            }
        }
        
        void practiceFretboardRecognition() const {
            std::cout << "\n=== Fretboard Note Recognition Exercise ===" << std::endl;
            std::cout << "For each position, identify the note on the fretboard." << std::endl;
            
            // Create a random seed
            std::srand(std::time(nullptr));
            
            for (int i = 1; i <= 5; ++i) {
                // Generate random string and fret
                int string = std::rand() % 6;  // 0-5 for the 6 strings
                int fret = std::rand() % 12;   // 0-11 for the first 12 frets
                
                std::cout << "Exercise " << i << ": What note is on string " << (6 - string) 
                        << " (counting from the lowest E string) at fret " << fret << "? ";
                
                std::string userAnswer;
                std::cin >> userAnswer;
                std::string correctNote = fretboard.getFretboard()[string][fret].getName();
                // Convert to uppercase for comparison
                std::transform(userAnswer.begin(), userAnswer.end(), userAnswer.begin(), ::toupper);
                
                // Get the correct note from the fretboard
                correctNote = fretboard.fretboard[string][fret].getName();
                std::transform(correctNote.begin(), correctNote.end(), correctNote.begin(), ::toupper);
                
                bool isCorrect = (correctNote.find(userAnswer) != std::string::npos) || 
                                (userAnswer.find(correctNote) != std::string::npos);
                
                if (isCorrect) {
                    std::cout << "Correct! " << fretboard.fretboard[string][fret].getName() << " is the note." << std::endl;
                } else {
                    std::cout << "Incorrect. The correct note is " << fretboard.fretboard[string][fret].getName() << "." << std::endl;
                }
            }
        }
        
        void practiceScalePatterns() const {
            std::cout << "\n=== Scale Pattern Exercise ===" << std::endl;
            std::cout << "Practice these common scale patterns on your guitar:" << std::endl;
            
            std::cout << "\n1. Major Scale - Position 1 (E shape)" << std::endl;
            std::cout << "E |--0--2--3--5--7--8--10--12--|\n"
                    << "B |--0--2--3--5--7--8--10--12--|\n"
                    << "G |--0--2--4--5--7--9--10--12--|\n"
                    << "D |--0--2--4--5--7--9--10--12--|\n"
                    << "A |--0--2--4--5--7--9--10--12--|\n"
                    << "E |--0--2--3--5--7--8--10--12--|\n";
            
            std::cout << "\n2. Minor Pentatonic - Box Position 1" << std::endl;
            std::cout << "E |--0--3--5--7--10--12--|\n"
                    << "B |--0--3--5--8--10--12--|\n"
                    << "G |--0--2--5--7--10--12--|\n"
                    << "D |--0--2--5--7--10--12--|\n"
                    << "A |--0--3--5--7--10--12--|\n"
                    << "E |--0--3--5--7--10--12--|\n";
            
            std::cout << "\nStart by practicing these patterns slowly, focusing on accuracy.\n"
                    << "Then gradually increase speed while maintaining clean playing.\n"
                    << "Try them in different keys by moving the patterns up and down the fretboard.\n";
            
            std::cout << "\nPress Enter to continue...";
            std::cin.ignore();
            std::cin.get();
        }
        
        void practiceChordConstruction() const {
            std::cout << "\n=== Chord Construction Challenge ===" << std::endl;
            std::cout << "Build the following chords by identifying the component notes:" << std::endl;
            
            // Create a random seed
            std::srand(std::time(nullptr));
            
            std::vector<std::string> chordTypes = {
                "Major", "Minor", "Dominant 7", "Major 7", "Minor 7"
            };
            
            for (int i = 1; i <= 3; ++i) {
                // Generate random root note
                int noteIndex = std::rand() % 12;
                Note rootNote(Note::ALL_NOTES[noteIndex], 60 + noteIndex);
                
                // Generate random chord type
                int typeIndex = std::rand() % chordTypes.size();
                std::string chordType = chordTypes[typeIndex];
                
                std::cout << "Exercise " << i << ": Construct a " << rootNote.getName() << " " << chordType << " chord." << std::endl;
                std::cout << "Enter the component notes separated by spaces: ";
                
                // Get user input
                std::string userInput;
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                std::getline(std::cin, userInput);
                
                // Create the correct chord
                Chord chord("", {}, rootNote);
                if (chordType == "Major") {
                    chord = Chord::major(rootNote);
                } else if (chordType == "Minor") {
                    chord = Chord::minor(rootNote);
                } else if (chordType == "Dominant 7") {
                    chord = Chord::dominant7(rootNote);
                } else if (chordType == "Major 7") {
                    chord = Chord::major7(rootNote);
                } else if (chordType == "Minor 7") {
                    chord = Chord::minor7(rootNote);
                }
                
                std::cout << "Correct answer: ";
                auto notes = chord.getNotes();
                for (const auto& note : notes) {
                    std::cout << note.getName() << " ";
                }
                std::cout << std::endl;
            }
        }
        
        void practiceChordProgressions() const {
            std::cout << "\n=== Chord Progression Practice ===" << std::endl;
            std::cout << "Practice these essential chord progressions in different keys:" << std::endl;
            
            std::cout << "\n1. I-IV-V (Blues/Rock progression)" << std::endl;
            std::cout << "   In C major: C - F - G" << std::endl;
            std::cout << "   In G major: G - C - D" << std::endl;
            std::cout << "   In D major: D - G - A" << std::endl;
            
            std::cout << "\n2. I-V-vi-IV (Pop progression)" << std::endl;
            std::cout << "   In C major: C - G - Am - F" << std::endl;
            std::cout << "   In G major: G - D - Em - C" << std::endl;
            std::cout << "   In D major: D - A - Bm - G" << std::endl;
            
            std::cout << "\n3. ii-V-I (Jazz progression)" << std::endl;
            std::cout << "   In C major: Dm7 - G7 - Cmaj7" << std::endl;
            std::cout << "   In F major: Gm7 - C7 - Fmaj7" << std::endl;
            std::cout << "   In Bb major: Cm7 - F7 - Bbmaj7" << std::endl;
            
            std::cout << "\n4. vi-IV-I-V (Melancholic progression)" << std::endl;
            std::cout << "   In C major: Am - F - C - G" << std::endl;
            std::cout << "   In G major: Em - C - G - D" << std::endl;
            std::cout << "   In D major: Bm - G - D - A" << std::endl;
            
            std::cout << "\nTips for practicing chord progressions:" << std::endl;
            std::cout << "- Start slow and focus on clean transitions between chords" << std::endl;
            std::cout << "- Practice with a metronome to develop timing" << std::endl;
            std::cout << "- Try different strumming patterns and rhythms" << std::endl;
            std::cout << "- Experiment with different voicings of the same chords" << std::endl;
            std::cout << "- Once comfortable, try to transpose to different keys" << std::endl;
            
            std::cout << "\nPress Enter to continue...";
            std::cin.ignore();
            std::cin.get();
        }
};

int main() {
    std::cout << "Welcome to the Guitar Music Theory Companion!" << std::endl;
    std::cout << "This application will help you explore music theory concepts on guitar." << std::endl;
    
    MusicTheoryCompanion companion;
    companion.showMainMenu();
    
    return 0;
}