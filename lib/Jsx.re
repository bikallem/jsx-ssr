/* This ppx converts the given jsx into a htmlElement instance. */

open Migrate_parsetree;
open Ast_407;

open Ast_mapper;
open Parsetree;
open Ast_helper;
open Asttypes;

let str_expr = s => [%expr [%e Exp.constant(Pconst_string(s, None))]];

let map_attribute_constant =
  fun
  | {pexp_desc: Pexp_ident({txt: Lident(ident), _}), pexp_attributes, _} as e => {
      let isRawLiteral = (
        fun
        | ({txt: "reason.raw_literal", _}, _) => true
        | _ => false
      );
      List.exists(isRawLiteral, pexp_attributes) ? str_expr(ident) : e;
    }
  | {pexp_desc: Pexp_constant(Pconst_integer(_)), _} as c => [%expr
      string_of_int([%e c])
    ]
  | {pexp_desc: Pexp_constant(Pconst_char(_)), _} as c => [%expr
      String.make(1, [%e c])
    ]
  | {pexp_desc: Pexp_constant(Pconst_float(_)), _} as c => [%expr
      string_of_float([%e c])
    ]
  | e => e;

let map_attributes = attributes => {
  List.fold_right(
    (attr, acc) =>
      switch (attr) {
      | (Nolabel, [%expr ()]) => acc
      | (Labelled("children"), _) => acc
      | (Labelled(propName), value) =>
        let key = str_expr(propName);
        let value = map_attribute_constant(value);
        %expr
        [Html.attr([%e key], [%e value]), ...[%e acc]];
      | (Nolabel, _)
      | (Optional(_), _) => failwith("Invalid attribute")
      },
    attributes,
    [%expr []],
  );
};

let rec map_children = e => {
  let map_constant =
    fun
    | {pexp_desc: Pexp_ident({txt: Lident(ident), _}), pexp_attributes, _} as e => {
        let isRawLiteral = (
          fun
          | ({txt: "reason.raw_literal", _}, _) => true
          | _ => false
        );
        List.exists(isRawLiteral, pexp_attributes)
          ? [%expr Html.text([%e str_expr(ident)])] : e;
      }
    | {pexp_desc: Pexp_constant(Pconst_integer(_)), _} as c => [%expr
        Html.text(string_of_int([%e c]))
      ]
    | {pexp_desc: Pexp_constant(Pconst_char(_)), _} as c => [%expr
        Html.text(String.make(1, [%e c]))
      ]
    | {pexp_desc: Pexp_constant(Pconst_float(_)), _} as c => [%expr
        Html.text(string_of_float([%e c]))
      ]
    | {pexp_desc: Pexp_constant(Pconst_string(_)), _} as c => [%expr
        Html.text([%e c])
      ]
    | e => e;

  switch (e) {
  | {
      pexp_desc:
        Pexp_construct(
          {txt: Lident("::"), _} as cons,
          Some({pexp_desc: Pexp_tuple(tuple), _} as a),
        ),
      _,
    } as e =>
    let tuple =
      switch (tuple) {
      | [{pexp_desc: Pexp_constant(_), _} as car, cdr] => [
          map_constant(car),
          map_children(cdr),
        ]
      | [car, cdr] => [car, map_children(cdr)]
      | x => x
      };

    {
      ...e,
      pexp_desc:
        Pexp_construct(cons, Some({...a, pexp_desc: Pexp_tuple(tuple)})),
    };
  | x => x
  };
};

/* Given a list of args, it returns a tuple of attributes and children. */
let map_args = args => {
  let attributes = map_attributes(args);
  let children =
    List.find(
      fun
      | (Labelled("children"), _) => true
      | _ => false,
      args,
    )
    |> snd
    |> map_children;
  (attributes, children);
};

let filter_map = (f, l) => {
  let rec recurse = (acc, l) =>
    switch (l) {
    | [] => List.rev(acc)
    | [x, ...l'] =>
      let acc' =
        switch (f(x)) {
        | None => acc
        | Some(y) => [y, ...acc]
        };
      recurse(acc', l');
    };

  recurse([], l);
};

let mapper = (_, _) => {
  let expr = (mapper, e) => {
    switch (e) {
    | {
        pexp_attributes: [({txt: "JSX", loc: _}, PStr([]))],
        pexp_desc:
          Pexp_apply(
            {pexp_desc: Pexp_ident({txt: Lident(html_tag), loc: _}), _},
            args,
          ),
        pexp_loc: _,
      } =>
      let (attributes, children) = map_args(args);

      %expr
      Html.element(
        [%e str_expr(html_tag)],
        [%e attributes],
        ~children=[%e default_mapper.expr(mapper, children)],
        (),
      );
    | {
        pexp_attributes: [({txt: "JSX", _}, PStr([]))],
        pexp_desc:
          Pexp_apply(
            {pexp_desc: Pexp_ident({txt: Ldot(_, "createElement"), _}), _} as expr,
            args,
          ),
        pexp_loc,
      } =>
      let (attributes, children) = map_args(args);
      let children = [%expr [%e default_mapper.expr(mapper, children)]];
      let args = [(Nolabel, attributes), (Labelled("children"), children)];
      Pexp_apply(expr, args) |> Exp.mk(~loc=pexp_loc);
    | e => default_mapper.expr(mapper, e)
    };
  };

  {...default_mapper, expr};
};

let () = Driver.register(~name="JSX", Versions.ocaml_407, mapper);
