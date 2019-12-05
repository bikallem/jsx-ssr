type t

val text : string -> t
val raw : string -> t
val comment : string -> t
val char : char -> t
val int : int -> t
val float : float -> t
val createElement : string -> Attribute.t list -> ?children:t list -> unit -> t
val pp : Format.formatter -> t -> unit
