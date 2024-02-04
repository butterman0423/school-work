#use "hw1.ml"

(** CASES **)

let gen_1 : fa = {
    states=["q0", "q1", "q2", "q3"];
    start="q0";
    tf=[("q0", 'm', "q1"), ("q1", 'e', "q1"), ("q1", 'a', "q2"), ("q2", 'w', "q3")];
    final=["q3"];    
}
let gen_2 : fa = {
    states=["A", "B", "C", "D"];
    start="A";
    tf=[("A", 'o', "B"), ("B", 'o', "B"), ("B", 'p', "C"), ("C", 'p', "D")];
    final=["D"];
}

let sin_1 : fa = {
    states=["q0"];
    start="q0";
    tf=[];
    final=["q0"];
}

let sin_2 : fa = {
    states=["q0"];
    start="q0";
    tf=[("q0", 'w', "q0")];
    final=["q0"];
}

let no_determ_1 : fa = {
    states=["q0", "q1", "q2", "q3"];
    start="q0";
    tf=[("q0", 'y', "q1"), ("q1", 'b', "q1"), ("q1", 'c', "q3"), ("q1", 'p', "q2")];
    final=["q2","q3"]
}
let no_determ_2 : fa = {
    states=["x","y","z"];
    start="x";
    tf=[("x",'1',"y"), ("x", '2', "y"), ("x", '3', "y"), ("y", '!', "z")];
    final["z"];
}

let inv_1 : fa = {
    states=["q0","q1","q2","q1","q0"];
    start="q0";
    tf=[];
    final=["q1","q2"];
}
let inv_2 : fa = {
    states=["a", "b", "c"];
    start="q5";
    tf=[];
    final=["c"];
}
let inv_3 : fa = {
    states=["a","b","c"];
    start="a";
    tf=[];
    final=["none"];
}
let inv_4 : fa = {
    states=["a","b","c", "d"];
    start="a";
    tf=[("a", 'j', "b"), ("a", 't', "d"), ("b", 'h', "c")]
    final=["d"];
}

let ded_1 : fa = {
    states=["q0","q1","q2","q3","q4", "q5"];
    start="q0";
    tf=[("q0", 'a', "q1"), ("q1", 'p', "q2"), ("q4", 'o', "q5"), ("q3", 'p', "q3")];
    final=["q2"];
}
let ded_2 : fa = {
    states=["q0","q1","q2","q3","q4"];
    start="q2";
    tf=[("q0", 'a', "q1"), ("q1", 'p', "q2"), ("q2", 'q' "q3") ,("q3", 'o', "q4"), ("q3", 'p', "q3")];
    final=["q4"];
}

(** END **)

let tcases = [
    ("apply_transition_funtion", ());
    ("apply_
]  
