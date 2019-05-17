open Ppxlib;

let expr = (~loc, ~path as _) => Ast_builder.Default.eint(~loc, 42);

let ext =
  Extension.declare(
    "test",
    Extension.Context.expression,
    Ast_pattern.(pstr(nil)),
    expr,
  );
let () = Driver.register_transformation("test", ~extensions=[ext]);
