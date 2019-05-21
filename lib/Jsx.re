/* This ppx just converts the given jsx into a html tag name expression. */

open Migrate_parsetree;
open Ast_406;

let ocaml_version = Versions.ocaml_406;

let mapper = (_, _) => {
  open Ast_mapper;
  open Parsetree;
  open Ast_helper;

  let expr = (mapper, e) => {
    switch (e) {
    | {
        pexp_attributes: [({txt: "JSX", loc: _}, PStr([]))],
        pexp_desc:
          Pexp_apply(
            {pexp_desc: Pexp_ident({txt: Lident(html_tag), loc: _}), _},
            _args,
          ),
        pexp_loc: _,
      } =>
      Exp.constant(Pconst_string(html_tag, None))
    | _ => default_mapper.expr(mapper, e)
    };
  };

  {...default_mapper, expr};
};

let () = Driver.register(~name="JSX", ocaml_version, mapper);
