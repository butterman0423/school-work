(* Quiz 1 - 31 January 2024

   Student name 1: Nathaniel Escaro
   Student name 2: Ryan Eshan
   
   I pledge my honor that I have abided by the Stevens Honor System.
*)


(* Notes: 
    a. You may add helper functions.
    b. "let rec" allows your function to be recursive, but it doesn't
    have to be. 
*)

(* Sample Directed Graph *)

let ex = [(1, 2); (2, 3); (3, 1); (3, 4)]


(*
  1 <------ 3
  |      //||
  |     /   | 
  |    /    | 
 \/  /     \/
  2        4
*)
       
(** [sub l1 l2] returns the list resulting from subtracting every 
    element in [l2] from [l1].
    Eg. sub [1;2] [] => [1; 2]
    Eg. sub [1;2;1] [1] => [2]
    Eg. sub [1;2] [2;3;1] => []
*)
let rec sub l1 l2 =
    match l2 with
    | [] -> l1
    | m::t -> sub (List.filter (fun n -> n<>m) l1) t
    
(** [outgoing g n] returns the list of all the nodes that are
    immediate neighbors of [n] in [g].
    Eg. outgoing ex 3 => [1,4] 
*)
let rec outgoing_nodes g n =
    match g with
    | [] -> []
    | (x, y)::t -> if n=x
                   then y :: outgoing_nodes t n
                   else outgoing_nodes t n

(** [nodes g] returns the list of nodes of the graph [g] without duplicates. 
   The order of the nodes in the list is irrelevant.
   eg. nodes ex => [1,2,3,4] 
*)
let rec nodes g =
    match g with
    | [] -> []
    | (x, y)::t -> let prev = nodes t
                   in let prev = (if List.mem x prev then prev else x :: prev)
                   in if List.mem y prev then prev else y :: prev

(** [degree g] returns the degree of [g]. The degree of a graph is 
    the maximum number of outgoing edges that any node has. 
*)
let rec degree g =
    match g with
    | [] -> 0
    | (x, _)::t -> let prev_edge = degree t
                   in let edges = outgoing_nodes g x
                   in if (List.length edges) > prev_edge
                      then List.length edges
                      else prev_edge

(** [remove g n] removes node [n] and all the edges involving [n], from
   the graph [g].
   Eg. remove ex 2 =>  [(3, 1); (3, 4)] 
*)
let rec remove g n =
    List.filter (fun (x, y) -> x<>n && y<>n) g

(** [reachable g n] returns the list of all the reachable nodes from
   node [n] in [g]. (Extra-credit)
   Eg. reachable ex 3 => [1,4,2,3] 
*)

let rec reachable g n =
    let ptns = List.filter (fun (x, y) -> x=n || y=n) g
    in let ptns = List.map (fun (x, y) -> if x=n then y else x) ptns
    in let rec iter = 
        (fun inp lst ->
            match inp with
            | [] -> []
            | h::t -> (f h (iter f t lst))
        )
    in iter (fun n lst -> if List.mem n lst then lst else n :: lst) ptns []
