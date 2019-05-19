/* This ppx just converts the given jsx into a html tag name expression. */

open Migrate_parsetree;
open Ast_406;

let ocaml_version = Versions.ocaml_406;

let mapper = (_, _) => {
  open Ast_mapper;
  open Parsetree;

  let expr = (mapper, e) => {
    switch (e) {
    | {
        pexp_attributes: [({txt: "JSX", _}, PStr([]))],
        pexp_desc:
          Pexp_apply(
            {pexp_desc: Pexp_ident({txt: Lident(html_tag), _}), _},
            _args,
          ),
        _,
      } =>
      Ast_helper.Exp.constant(Pconst_string(html_tag, None))
    | _ => default_mapper.expr(mapper, e)
    };
  };

  {...default_mapper, expr};
};

let () = Driver.register(~name="ppx-jsx", ocaml_version, mapper);
