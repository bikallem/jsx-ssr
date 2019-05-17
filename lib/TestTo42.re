open Ppxlib;

let expr = (~loc, ~path as _) => Ast_builder.Default.eint(~loc, 42);

let name = "test";

let ext =
  Extension.declare(
    name,
    Extension.Context.expression,
    Ast_pattern.(pstr(nil)),
    expr,
  );
let () = Driver.register_transformation(name, ~extensions=[ext]);
