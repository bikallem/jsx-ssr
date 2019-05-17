open Ppxlib;

let name = "JSX";

let to42 = (~name_loc, _e) => {
  Ast_builder.Default.eint(~loc=name_loc, 42);
};

let ext =
  Attribute.declare_with_name_loc(
    name,
    Attribute.Context.expression,
    Ast_pattern.(single_expr_payload(__)),
    to42,
  );
