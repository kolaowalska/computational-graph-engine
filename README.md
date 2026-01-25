# computational-graph-engine

## overview & purpose
this project is a lightweight C++ 20 framework for building and evaluating mathematical expressions using directed acyclic graphs. it leverages static polymorphism and concepts to accommodate a type-safe environment with support for standard arithmetic, forward automatic differentiation, and structural graph optimizations.

### general functionalities

- __polymorphic node hierarchy__: abstract base classes for unary, binary, input, and constant nodes
- __automatic differentiation__: built-in support for dual numbers to compute exact derivatives alongside function values
- __common subexpression elimination__: automatic detection and reuse of idential subcalculations to reduce memory overhead and computation time
- __fluent dsl__: operator overloading allows for writing math naturally
- __swappable evaluation strategies__
- __graph visualization__

### structure tour d'horizon 

| entity       |  element  | purpose                                                                                                   |
|:-------------|:---------:|-----------------------------------------------------------------------------------------------------------|
| `Graph`      | structure | owns the lifecycle of nodes and ensures the graph's structural integrity                                  |
| `Expression` |   logic   | a wrapper providing the user-facing interface and operator overloads                                      |
| `Evaluator`  | algorithm | reading graphs, <br/> eparates *how* the graph is traversed from the graph itself using template policies |


## class & module responsibilities 

### 1. core structure: `include/cg/graph.hpp`, `include/cg/node.hpp`

#### class `Graph<T>`
- **role:** the centralized owner and container of the computational structure
- **responsibilities:**
    - owns all `Node` objects via `std::unique_ptr`
    - provides factory methods like `constant`, `input`, `add` to ensure valid graph construction
    - maintains the topological integrity of the DAG
- **abstraction / design choice:**
    - `template<T>` allows the entire engine to operate on any numeric type without code duplication
    - container abstraction ensures that the user doesn't manage `Node*` pointers directly and can use safe `NodeID` handles instead

#### class `Node<T>` (abstract base class)
- **role:** the polymorphic interface for any operation or data source in the graph
- **responsibilities:**
    - defines the contract that all nodes must satisfy
    - provides runtime type information via `kind()` and `label()` for visualization
- **abstraction / design choice:**
    - runtime polymorphism through virtual functions allows the `Graph` and `Evaluator` to treat nodes uniformly
    - `Numeric T` concepts ensure at compile time that the underlying data type supports all necessary math operations

#### struct `NodeID`
- **role:** a puny, strongly-typed handle to a node
- **responsibilities:**
    - wraps a `std::size_t` index to prevent misuse
- **abstraction / design choice:**
    - strong typing for primitive obsession bugs

### 2. ui/dsl: `include/cg/expression.hpp`

#### class `Expression<T>`
- **role:** a high-level wrapper that enables the domain specific language
- **responsibilities:**
    - wraps a `NodeID` and a reference to the `Graph`
    - enables operator overloading
- **abstraction / design choice:**
    - facade pattern hides the complexity of graph construction
    - acts as a view; doesn't own the data, preventing ownership cycles

### 3. all the math: `include/cg/ops.hpp`, `include/cg/dual.hpp`

#### structs `ops::Add`, `ops::Mul`, `ops::Sin`, etc.
- **role:** stateless functors defining the mathematical logic
- **responsibilities:**
    - provide the actual computation `operator()(T x, T y)`
    - deliver visualization metadata
- **abstraction / design choice:**
    - static polymprohism and functors separate the operation logic from the `Node` structure, so the generic `BinaryNode<T, Op>` can be reused for addition, subtraction, etc. preventing code redundancy

#### class `Dual<T>`
- **role:** a custom numeric type for FAD
- **responsibilities:**
    - stores a value pair `{value, d}`
    - implements the chain rule <3 via operator overloading
- **abstraction / design choice:**
    - thanks to the swappable domain (`Graph<T>` template), one can simply change `T` from `double` to `Dual` and the engine upgrades from a calculator to a differentiatior without changing a single line of code code

### 4. evaluation engine: `include/cg/eval/`

#### class `Evaluator<T, Policy>`
- **role:** the execution context that delegates how compuation happens to a `Policy`
- **responsibilities:**
    - provides a consistent API to the user
- **abstraction / design choice:**
    - the compile-time strategy pattern hosts swappable algorithms by changing a template argument

#### policies `NaiveEvaluator`, `LazyEvaluator`
- **role:** concrete implementations of an evaluation strategy
- **responsibilities:**
    - performs the actual graph traversal (topological sort or recursive DFS)
    - manages the `Context` (variable lookups)
- **abstraction / design choice:**
    - separation of concerns for adding new evaluation methods wihtout having to meddle with the `Graph` code

## quick start

### prerequisites
- C++20 compliant compiler (GCC 10+, Clang 10+, or MSVC 19.28+)
- CMake 3.20+
- *[optional]* Graphviz for PNG rendering 

### running the demo
1. clone or download the repository to your local machine
2. open the terminal and navigate to the project's root directory
3. run the following commands:
```bash
mkdir build && cd build
cmake ..
make 
./main
```

