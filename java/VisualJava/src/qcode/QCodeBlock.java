interface QCodeBlock implements QIO {
    // We hold not a tree, but an ordered forest?
    List<QStatementObj> children;
}

// The "parent"
interface QStatementObj implements QIO {
}

// There are two types of QStatementObj:
// A QStatement: a single operation, depth 0
// A QSpecialStatement: variable depth, needs renaming
//   things like if and while statements, which contain one or more QCodeBlocks.

// Where do anon functions come into this?  They may be placed
// into a variable in a QStatement, or created and immediately
// consumed anonymously during a function call.

interface QStatement implements QStatementObj {
}

interface QSpecialStatement implements QStatementObj {
}



interface QIfStatement implements QSpecialStatement {
}

interface QLoop implements QSpecialStatement {
}
interface QForLoop implements QLoop {
}
interface QWhileLoop implements QLoop {
}

interface QFunctionCall implements QStatement {

}

class QAssignment implements QStatement {
    // var name
    // expression
}

class QExpression {
}



