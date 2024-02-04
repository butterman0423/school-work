let rec mem : 'a -> 'a list -> bool =
    fun e l ->
    match l with
    | [] -> false
    | h::t -> (e=h) || mem e t

let rec has_duplicates : 'a list -> bool = 
    fun l ->
    match l with
    | [] -> false
    | h::t -> (mem h t) || has_duplicates t

let rec last : 'a list -> 'a =
   func l ->
   match l with
   | [] -> failwith "last: empty list"
   | [h] -> h
   | h1::h2::t -> last (h2::t)
