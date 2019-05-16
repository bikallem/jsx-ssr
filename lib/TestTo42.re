open Ppxlib;

let expr = (~loc, ~path as _, e) =>
  switch (e.pexp_desc) {
  | Pexp_extension(({txt: "test", _}, _payload)) =>
    Ast_builder.Default.eint(~loc, 42)
  | _ => e
  };

let ext =
  Extension.declare(
    "test",
    Extension.Context.expression,
    Ast_pattern.(single_expr_payload(__)),
    expr,
  );
let () = Driver.register_transformation("test", ~extensions=[ext]);
