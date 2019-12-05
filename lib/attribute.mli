type t

val attr : string -> string -> t
val flag : string -> t
val pp : Format.formatter -> t -> unit
