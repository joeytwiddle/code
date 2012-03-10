package qcode;

// Beware: Currently a QCode is associated with a function.  (Could potentially have backlinks/metadata.)
// Context agnostic code is a QCodeBlock?

abstract class QCode implements QIO {

    QCodeBlock codeRoot;

}
