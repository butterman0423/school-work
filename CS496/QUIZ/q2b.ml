(* 
     Quiz 2 - 7 Feb 2024
     Name1: Nathaniel Escaro 
     Name2: Ryan Eshan
 *)

type 'a bt = Empty |  Node of 'a * 'a bt * 'a bt

(* Helper function that builds a leaf *)
let leaf n = Node(n,Empty,Empty)

(* Two sample binary trees.
   The first one is a BST, but not the second one *)
let t1 : int bt = Node(12,
                       Node(7,Empty,leaf 10),
                       Node(24,
                            leaf 14,
                            Empty))

let t2 : int bt = Node(12,
                       leaf 7,
                       Node(24,
                            leaf 30,
                            Empty))

(** returns smallest element in non-empty tree [t].
    Fails if [t] is empty, it should fail. 
    Note: the tree [t] is not assumed to be a bst *)
let rec mint : 'a bt -> 'a =
  fun t ->
      match t with
      | Empty -> failwith "Empty tree"
      | Node(item, Empty, Empty) -> item
      | Node(item, Empty, rht) -> min item (mint rht)
      | Node(item, lft, Empty) -> min item (mint lft)
      | Node(item, lft, rht) -> min item (min (mint lft) (mint rht))
  
(** returns largest element in non-empty tree [t].
    Fails if [t] is empty, it should fail. 
    Note: the tree [t] is not assumed to be a bst *)
let rec maxt : 'a bt -> 'a =
  fun t ->
      match t with
      | Empty -> failwith "Empty tree"
      | Node(item, Empty, Empty) -> item
      | Node(item, Empty, rht) -> max item (maxt rht)
      | Node(item, lft, Empty) -> max item (maxt lft)
      | Node(item, lft, rht) -> max item (max (maxt lft) (maxt rht))

 
let rec is_bst : 'a bt -> bool =
    fun t ->
        match t with
        | Empty -> true
        | Node(k, Empty, Empty) -> true
        | Node(k, Node(v, lft, rht), Empty) -> k > v && is_bst (Node(v, lft, rht))
        | Node(k, Empty, Node(v, lft, rht)) -> k < v && is_bst (Node(v, lft, rht))
        | Node(k, Node(v, llft, lrht), Node(w, rlft, rrht))  -> 
                k > v && v < w && is_bst llft && is_bst lrht && is_bst rlft && is_bst rrht

(** [add k t] adds key [k] to the bst [t]. 
    Should fail with failwith if [k] is already in the tree.
    Otherwise, returns updated tree *)
let rec ad : 'a -> 'a bt -> 'a bt =
    fun k t ->
        match t with
        | Empty -> leaf k
        | Node(item, Empty, Empty) when item > k -> Node(item, leaf k, Empty)
        | Node(item, Empty, Empty) when item < k -> Node(item, Empty, leaf k)
        | Node(item, lft, rht) when item > k -> Node(item, (ad k lft), rht)
        | Node(item, lft, rht) when item < k -> Node(item, lft, (ad k rht))

(** [rem k t] removes key [v] from the BST [t] 
   Should fail with failwith if [k] is not in tree
   (Extra-credit) *)
let rec rem : 'a -> 'a bt -> 'a bt =
   fun k t ->
   failwith "implement"
 





