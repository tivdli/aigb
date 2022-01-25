var =    """static int *tr_2;
    static int *tr_1;
    static int *ts_d;
    static int *ts_n;
    static int *hr_1;
    static int *hr_2;
    static int *hs_d;
    static int *hs_n;
    static int *cr_1;
    static int *lr_1;
    static int *lr_2;
    static int *wr_c;
    static int *ps_p;

    static int *ls_p;
    static int *light_color_setting;

    static int *fs_i;
    static int *fs_v;
    static bool *ps_o;
    static bool *ls_o;
    static bool *fs_o;
    static bool *as_o;"""

var = var.replace("static", "")
var = var.translate({ord(" "): None})
var = var.replace("\n", "")
var = var.replace(";",",")
var = var.replace("bool", "")
var = var.replace("int", "")
var = var.replace("*", " &")
print(var)
var = var.split()
c = 0
for i in range(0, len(var)):
    c+=1
    if c==3:
        var[i] = var[i]

print(var)