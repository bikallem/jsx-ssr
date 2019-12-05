type t [@@ocaml.doc " Element represents a HtmlJSX view engine. "]

val text : string -> t
val rawText : string -> t
val emptyText : t
val comment : string -> t
val char : char -> t
val int : int -> t
val float : float -> t
val createElement : string -> Attribute.t list -> ?children:t list -> unit -> t
val renderAsDoc : t -> string
val render : t -> string
