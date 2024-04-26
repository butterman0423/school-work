(**
   Name: Nathaniel Escaro
   Pledge: I pledge my honor that I have abided by the Stevens Honor System.
**)

type dTree = | Leaf of int | Node of char*dTree*dTree

let tLeft : dTree =
    Node('w',
        Node('x', Leaf(2), Leaf(5)),
        Leaf(8)
    )

let tRight : dTree = 
    Node('w',
        Node('x', Leaf(2), Leaf(5)),
        Node('y', Leaf(7), Leaf(5))
    )

(** Returns the height of a dTree **)
let rec height : dTree -> int =
    fun t ->
        match t with
        | Leaf(_) -> 1
        | Node(_, lft, rht) -> 
                let lh = height lft in
                let rh = height rht in
                if lh > rh then lh + 1 else rh + 1

(** Returns the size of a dTree **)
let rec size : dTree -> int =
    fun t ->
        match t with
        | Leaf(_) -> 1
        | Node(_, lft, rht) -> 1 + size lft + size rht

(** Returns a list with all the paths to a dTree's leaves **)
let rec paths : dTree -> int list list = 
    fun t ->
        match t with
        | Leaf(_) -> [[]]
        | Node(_, lft, rht) ->
                (List.map ((List.cons) 0) (paths lft)) @ (List.map ((List.cons) 1) (paths rht))

(** Determines whether a dTree is perfect
    i.e all leaves have the same depth **)
let is_perfect : dTree -> bool = 
    fun t ->
        let lens = List.map (List.length) (paths t) in
        not @@ List.exists ((<>) @@ List.hd lens) lens

(** Returns a new dTree by applying f to the characters
    and g to the numbers in each node/leaf **)
let rec map : (char -> char) -> (int -> int) -> dTree -> dTree =
    fun f g t ->
        match t with
        | Leaf(n) -> Leaf(g n)
        | Node(sym, lft, rht) -> Node(f sym, map f g lft, map f g rht)

(** Creates a tree from a char list where each
    nth symbol corresponds to an inner node at
    level n **)
let rec list_to_tree : char list -> dTree =
    fun lst ->
        match lst with
        | [] -> Leaf(0)
        | sym::t -> Node(sym, list_to_tree t, list_to_tree t)

(** Set all leaf values in the list of (paths, val) of a
    given dTree **)

(** HELPER **)

let crm_hd : int -> (int list * int) -> (int list * int) =
    fun n (lst, v) ->
        if List.hd lst = n
        then (List.tl lst, v)
        else (lst, v)

(** END **)

let rec replace_leaf_at : dTree -> (int list * int) list -> dTree =
    fun tr prs ->
        match tr with
        | Leaf(_) -> (fun (_, v) -> Leaf(v)) @@
                     List.hd @@
                     List.filter (fun (lst, _) -> lst=[]) prs
        | Node(sym, lft, rht) -> 
                Node(sym,
                    replace_leaf_at lft @@ List.map ((crm_hd) 0) prs,
                    replace_leaf_at rht @@ List.map ((crm_hd) 1) prs
                )

(** Constructs a dTree from a char list and a list of pair-encodings **)
let bf_to_dTree : char list * (int list * int) list -> dTree =
    fun (syms, lst) ->
        let tr = list_to_tree syms in
        replace_leaf_at tr lst
