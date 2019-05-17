open Ppxlib;

let name = "JSX";

let id = x => {
  print_endline("inside JSX ppx");
  x;
};

let ext =
  Attribute.declare(
    name,
    Attribute.Context.expression,
    Ast_pattern.(single_expr_payload(__)),
    id,
  );
