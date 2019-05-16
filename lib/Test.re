open Ppxlib;

let expr = (~loc, ~path as _, _env: string) =>
  switch (e.pexp_desc) {
  | Pexp_extension(({txt: "test", _}, _payload)) =>
    Ast_helper.Exp.constant(Pconst_integer("42", None))
  | _ => default_mapper.expr(mapper, e)
  };

let ext = 
  Extension.declare("test", 
                    Extension.Context.expression,
                    Ast_pattern.(single_expr_payload(estring(__))),
                    expr);
let () = Driver.register_transformation("test", ~extensions=[ext]);
