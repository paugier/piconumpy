from transonic.util import timeit


def timeit_verbose(
    stmt,
    setup="pass",
    total_duration=2,
    globals=None,
    norm=None,
    name=None,
    print_time=False,
    max_length_name=33,
):
    result = timeit(
        stmt, setup=setup, total_duration=total_duration, globals=globals
    )
    if norm is None:
        norm = result
        norm_given = False
    else:
        norm_given = True

    fmt_name = f"{{:{max_length_name}s}}"
    if name is None:
        name = stmt.split("(")[0]

    name = fmt_name.format(name)

    if print_time:
        raw_time = f" = {result:4.3g} s"
    else:
        raw_time = ""

    print(f"{name}: {result/norm:4.3g} * norm{raw_time}")
    if not norm_given and not print_time:
        print(f"norm = {norm:4.3g} s")

    return result
