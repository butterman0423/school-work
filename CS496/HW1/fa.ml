(**
Name: Nathaniel Escaro
Pledge: I pledge my honor that I have abided by the Stevens Honor System.
 **)

(* ******************************************** *)
(** Basic functions on finite automata *)
(* ******************************************** *)
(**
Code stub for assignment 1
*)
type symbol = char
type input = char list

type state = string

(* transition function *)
type tf = (state * symbol * state) list

(* initial state * transition function * end state *)
type fa = { states: state list; start:state; tf: tf; final: state list}

(* ******************************************** *)
(* Examples of automata *)
(* ******************************************** *)
let a = { states = ["q0";"q1";"q2"];
          start = "q0";
          tf = [("q0",'a',"q1"); ("q1",'b',"q1"); ("q1",'c',"q2")];
          final = ["q2"]}

let a2 = { states = ["q0";"q1";"q2";"q3";"q4"];
           start = "q0";
           tf = [("q0",'a',"q1"); ("q1",'b',"q1"); ("q1",'c',"q2"); ("q3",'a',"q4")];
           final= ["q2"]}

let tf_of_a = [("q0",'a',"q1"); ("q1",'b',"q1"); ("q1",'c',"q2")]

(* ******************************************** *)
(* Helper functions *)
(* ******************************************** *)

let input_of_string s =
    let rec exp i l =
        if i < 0 then l else exp (i - 1) (s.[i] :: l) in
    exp (String.length s - 1) []

(** MY HELPERS **)

(** Overwrite start field in fa with state **)
let ow_start : fa -> state -> fa =
    fun a st ->
        {
            states = a.states;
            start = st;
            tf = a.tf;
            final = a.final;
        }

(** Overwrite tf field in fa with tf **)
let ow_tf : fa -> tf -> fa =
    fun a f ->
        {
            states = a.states;
            start = a.start;
            tf = f;
            final = a.final;
        }

(** fold_right (I think) but accumulator is a list **)
let rec rfold_lst : ('a -> 'acc list -> 'acc list) -> 'a list -> 'acc list -> 'acc list =
    fun f inp lst ->
        match inp with
        | [] -> lst
        | h::t -> (f h (rfold_lst f t lst))


(* ******************************************** *)
(* Simulating automata *)
(* ******************************************** *)

(**
Param: f st sym
Applies f to the symbol sym assuming that the current state is st
Assume that FA is deterministic
 **)
let rec apply_transition_function : tf -> state -> symbol -> state option =
    fun f st sym ->
        match f with
        | [] -> None
        | (start, check, final)::t ->
                if start=st && check=sym
                then Some final
                else apply_transition_function t st sym

(**
Determine whether a word is accepted by a finite automation
 **)
let rec accept : fa -> input -> bool =
    fun a word ->
        match word with
        | [] -> List.mem a.start a.final
        | curr::sub -> 
                let next_st = apply_transition_function a.tf a.start curr
                in match next_st with
                | None -> false
                | st -> accept (ow_start a (Option.get st)) sub

(**
Returns the list of all the states that are successors
of some given state with some give symbol.
 **)
let rec next : tf -> state -> symbol -> state list =
    fun f st sym ->
        match f with
        | [] -> []
        | (start, check, final)::t ->
                if st=start && sym=check
                then final :: next t st sym
                else next t st sym 

(**
Checks whether fa is non-deterministic or not.

Non-deterministic FA is one in which there is a state
that has two or more successor states with the same symbol.
 **)
let rec deterministic : fa -> bool =
    fun a ->
        match a.tf with
        | [] -> true
        | (st1, sy1, _)::__ ->
                (** Filt contains states that have starts and syms not equal to h **)
                let filt = List.filter (fun (st2, sy2, _) -> st1<>st2 || sy1<>sy2) a.tf
                in if ( (List.length a.tf) - (List.length filt) ) > 1
                then false
                else deterministic (ow_tf a filt)  

(**
Checks if a FA is valid:
1) List of states has no dupes
2) Start state belongs to set of states
3) Final states belong to set of states
4) Is deterministic
 **)
let valid : fa -> bool =
    fun a ->
        List.for_all (fun st -> List.length (List.filter ((=) st) a.states) = 1) a.states &&
        List.mem a.start a.states &&
        List.for_all (fun st -> List.mem st a.states) a.final &&
        deterministic a

(**
Reports a list of states that are reachable from the start state.
 **)
let rec reachable : fa -> state list =
    fun a ->
        if List.mem a.start a.final
        then a.start :: []
        else 
            let sts = List.filter (fun (st, _, fi) -> st=a.start && st<>fi) a.tf
            in let sts = List.map (fun (_, __, fi) -> fi) sts
            in rfold_lst
                (fun st lst -> 
                    if List.mem st lst 
                    then lst
                    else lst @ reachable (ow_start a st)
                ) sts (a.start :: [])

(**
Determines whether a FA accepts at least one word
 **)
let non_empty : fa -> bool =
    fun a ->
        let rsts = reachable a
        in List.exists (fun st -> List.mem st a.final) rsts

(**
Removes all dead (unreachable) states from a valid FA
 **)
let remove_dead_states : fa -> fa =
    fun a ->
        let rsts = reachable a
        in let is_in = (fun st -> List.mem st rsts)
        in {
            states = List.filter is_in a.states;
            start = a.start;
            tf = List.filter (fun (st, _, fi) -> is_in st && is_in fi) a.tf;
            final = List.filter is_in a.final;
        }
