#include <stdlib.h>
#include <stdio.h>
#include <math.h>

struct array_prob {
  int size; double * data;
};

struct array_nat {
  int size; unsigned int * data;
};

struct mdata_nat {
  double weight; unsigned int sample;
};

struct dat_DDV {
  int index;
};

double logSumExp2(double a, double b)
{
  return (a > b) ? (a + log1p((expm1((b - a)) + 1))) : (b + log1p((expm1((a - b)) + 1)));
}

double logSumExp3(double a, double b, double c)
{
  return (a > b) ? (a > c) ? (a + log1p(((expm1((c - a)) + expm1((b - a))) + 2))) : (c + log1p(((expm1((b - c)) + expm1((a - c))) + 2))) : (b > c) ? (b + log1p(((expm1((c - b)) + expm1((a - b))) + 2))) : (c + log1p(((expm1((b - c)) + expm1((a - c))) + 2)));
}

struct mdata_nat gibbsC(struct array_prob topic_prior_param_a, struct array_prob word_prior_param_b, struct array_nat z_param_c, struct array_nat w_param_d, struct array_nat doc_param_e, unsigned int docUpdate_param_f)
{
  struct mdata_nat out_a;
  unsigned int __b;
  struct array_nat _c;
  unsigned int __d;
  struct array_nat _e;
  unsigned int __f;
  unsigned int lo_g;
  unsigned int hi_h;
  unsigned int iu19993__i;
  unsigned int acc_j;
  double __x;
  unsigned int _y;
  double p_z;
  unsigned int __a0;
  struct array_prob _a1;
  double __a2;
  unsigned int lo_a3;
  unsigned int hi_a4;
  struct array_prob summate_arr_a5;
  double maxV_a6;
  double sum_a8;
  unsigned int maxI_a7;
  unsigned int iu19993__a9;
  unsigned int __ad;
  struct array_prob _ae;
  double __af;
  unsigned int lo_ag;
  unsigned int hi_ah;
  struct array_prob summate_arr_ai;
  double maxV_aj;
  double sum_al;
  unsigned int maxI_ak;
  unsigned int iu19993__am;
  double __aq;
  double _ar;
  double _as;
  double __at;
  double _au;
  struct array_prob __av;
  unsigned int zNewu19983__aw;
  struct array_prob _gx;
  int it_gy;
  double ws_gz;
  double max_h0;
  double r_h1;
  _c = w_param_d;
  __b = _c.size;
  _e = z_param_c;
  __d = _e.size;
  lo_g = 0;
  hi_h = __d;
  /* ---------- Begin Summate ---------- */
  acc_j = 0;
  for (iu19993__i = lo_g; iu19993__i < hi_h; iu19993__i++)
  {
    unsigned int _k;
    unsigned int __l;
    unsigned int _m;
    struct array_nat _n;
    struct dat_DDV __o;
    unsigned int _p;
    unsigned int _q;
    unsigned int __r;
    struct dat_DDV _s;
    struct dat_DDV __t;
    unsigned int _u;
    unsigned int _v;
    struct dat_DDV _w;
    _m = iu19993__i;
    _n = z_param_c;
    __l = _n.data[_m];
    _p = __l;
    _q = 0;
    __o.index = (_p < _q) ? 0 : 1;
    _s = __o;
     if ((_s.index == 0))
     {
       __r = 0;
     }
     else
     {
       __r = 1;
     }
    _u = iu19993__i;
    _v = docUpdate_param_f;
    __t.index = (_u == _v) ? 0 : 1;
    _w = __t;
     if ((_w.index == 0))
     {
       _k = 0;
     }
     else
     {
       _k = __r;
     }
    acc_j += _k;
  }
  __f = acc_j;
  /* ----------- End Summate ----------- */
  _y = __f;
  p_z = log(_y);
  __x = p_z;
  _a1 = word_prior_param_b;
  __a0 = _a1.size;
  lo_a3 = 0;
  hi_a4 = __a0;
  /* ---------- Begin Summate ---------- */
  summate_arr_a5.size = (hi_a4 - lo_a3);
  summate_arr_a5.data = ((double *)malloc((summate_arr_a5.size * sizeof(double))));
  for (iu19993__a9 = 0; iu19993__a9 < summate_arr_a5.size; iu19993__a9++)
  {
    double _aa;
    unsigned int _ab;
    struct array_prob _ac;
    _ab = iu19993__a9;
    _ac = word_prior_param_b;
    _aa = _ac.data[_ab];
    summate_arr_a5.data[iu19993__a9] = _aa;
    if (((maxV_a6 < _aa) || (iu19993__a9 == 0)))
    {
      maxV_a6 = _aa;
      maxI_a7 = iu19993__a9;
    }
  }
  sum_a8 = 0.0;
  for (iu19993__a9 = 0; iu19993__a9 < summate_arr_a5.size; iu19993__a9++)
  {
    if ((iu19993__a9 != maxI_a7))
     sum_a8 += exp((summate_arr_a5.data[iu19993__a9] - maxV_a6));
  }
  __a2 = (maxV_a6 + log1p(sum_a8));
  free(summate_arr_a5.data);
  /* ----------- End Summate ----------- */
  _ae = topic_prior_param_a;
  __ad = _ae.size;
  lo_ag = 0;
  hi_ah = __ad;
  /* ---------- Begin Summate ---------- */
  summate_arr_ai.size = (hi_ah - lo_ag);
  summate_arr_ai.data = ((double *)malloc((summate_arr_ai.size * sizeof(double))));
  for (iu19993__am = 0; iu19993__am < summate_arr_ai.size; iu19993__am++)
  {
    double _an;
    unsigned int _ao;
    struct array_prob _ap;
    _ao = iu19993__am;
    _ap = topic_prior_param_a;
    _an = _ap.data[_ao];
    summate_arr_ai.data[iu19993__am] = _an;
    if (((maxV_aj < _an) || (iu19993__am == 0)))
    {
      maxV_aj = _an;
      maxI_ak = iu19993__am;
    }
  }
  sum_al = 0.0;
  for (iu19993__am = 0; iu19993__am < summate_arr_ai.size; iu19993__am++)
  {
    if ((iu19993__am != maxI_ak))
     sum_al += exp((summate_arr_ai.data[iu19993__am] - maxV_aj));
  }
  __af = (maxV_aj + log1p(sum_al));
  free(summate_arr_ai.data);
  /* ----------- End Summate ----------- */
  _ar = __x;
  _as = __af;
  __aq = logSumExp2(_ar,_as);
  _au = __aq;
  __at = (-_au);
  __av.size = __ad;
  __av.data = ((double *)malloc((__av.size * sizeof(double))));
  /* ----------- Begin Array ----------- */
  for (zNewu19983__aw = 0; zNewu19983__aw < __av.size; zNewu19983__aw++)
  {
    double __ax;
    unsigned int lo_ay;
    unsigned int hi_az;
    unsigned int i__b2;
    double t_b0;
    double c_b1;
    double __de;
    double _df;
    double __dg;
    unsigned int _dh;
    struct array_prob _di;
    unsigned int __dj;
    unsigned int lo_dk;
    unsigned int hi_dl;
    unsigned int iu19993__dm;
    unsigned int acc_dn;
    double __e1;
    unsigned int _e2;
    double p_e3;
    double __e4;
    double _e5;
    double _e6;
    double __e7;
    unsigned int lo_e8;
    unsigned int hi_e9;
    unsigned int i__ec;
    double t_ea;
    double c_eb;
    double _gt;
    double _gu;
    double _gv;
    double _gw;
    lo_ay = 0;
    hi_az = __ad;
    /* ---------- Begin Product ---------- */
    t_b0 = 0.0;
    c_b1 = 0.0;
    for (i__b2 = lo_ay; i__b2 < hi_az; i__b2++)
    {
      double x_b3;
      double y_b4;
      double z_b5;
      unsigned int __b6;
      unsigned int lo_b7;
      unsigned int hi_b8;
      unsigned int iu19993__b9;
      unsigned int acc_ba;
      double __c3;
      unsigned int _c4;
      double p_c5;
      struct dat_DDV __c6;
      unsigned int _c7;
      unsigned int _c8;
      unsigned int __c9;
      unsigned int lo_ca;
      unsigned int hi_cb;
      unsigned int iu19993__cc;
      unsigned int acc_cd;
      unsigned int lo_d0;
      unsigned int hi_d1;
      unsigned int iu20003__d4;
      double t_d2;
      double c_d3;
      lo_b7 = 0;
      hi_b8 = __b;
      /* ---------- Begin Summate ---------- */
      acc_ba = 0;
      for (iu19993__b9 = lo_b7; iu19993__b9 < hi_b8; iu19993__b9++)
      {
        unsigned int _bb;
        unsigned int __bc;
        unsigned int _bd;
        struct array_nat _be;
        unsigned int __bf;
        unsigned int _bg;
        struct array_nat _bh;
        struct dat_DDV __bi;
        unsigned int _bj;
        unsigned int _bk;
        unsigned int __bl;
        unsigned int _bm;
        struct array_nat _bn;
        struct dat_DDV __bo;
        unsigned int _bp;
        unsigned int _bq;
        struct dat_DDV __br;
        struct dat_DDV _bs;
        struct dat_DDV _bt;
        struct dat_DDV __bu;
        struct dat_DDV _bv;
        struct dat_DDV _bw;
        unsigned int __bx;
        struct dat_DDV _by;
        struct dat_DDV __bz;
        unsigned int _c0;
        unsigned int _c1;
        struct dat_DDV _c2;
        _bd = iu19993__b9;
        _be = doc_param_e;
        __bc = _be.data[_bd];
        _bg = __bc;
        _bh = z_param_c;
        __bf = _bh.data[_bg];
        _bj = i__b2;
        _bk = __bf;
        __bi.index = (_bj == _bk) ? 0 : 1;
        _bm = iu19993__b9;
        _bn = w_param_d;
        __bl = _bn.data[_bm];
        _bp = __bl;
        _bq = 0;
        __bo.index = (_bp < _bq) ? 0 : 1;
        _bs = __bo;
        _bt.index = (_bs.index == 1) ? 0 : 1;
        __br = _bt;
        _bv = __br;
        _bw = __bi;
        __bu.index = (!(_bw.index == _bv.index));
        _by = __bu;
         if ((_by.index == 0))
         {
           __bx = 1;
         }
         else
         {
           __bx = 0;
         }
        _c0 = __bc;
        _c1 = docUpdate_param_f;
        __bz.index = (_c0 == _c1) ? 0 : 1;
        _c2 = __bz;
         if ((_c2.index == 0))
         {
           _bb = 0;
         }
         else
         {
           _bb = __bx;
         }
        acc_ba += _bb;
      }
      __b6 = acc_ba;
      /* ----------- End Summate ----------- */
      _c4 = __b6;
      p_c5 = log(_c4);
      __c3 = p_c5;
      _c7 = i__b2;
      _c8 = zNewu19983__aw;
      __c6.index = (_c7 == _c8) ? 0 : 1;
      lo_ca = 0;
      hi_cb = __b;
      /* ---------- Begin Summate ---------- */
      acc_cd = 0;
      for (iu19993__cc = lo_ca; iu19993__cc < hi_cb; iu19993__cc++)
      {
        unsigned int _ce;
        unsigned int __cf;
        unsigned int _cg;
        struct array_nat _ch;
        struct dat_DDV __ci;
        unsigned int _cj;
        unsigned int _ck;
        struct dat_DDV __cl;
        struct dat_DDV _cm;
        struct dat_DDV _cn;
        struct dat_DDV __co;
        struct dat_DDV _cp;
        struct dat_DDV _cq;
        unsigned int __cr;
        struct dat_DDV _cs;
        unsigned int __ct;
        unsigned int _cu;
        struct array_nat _cv;
        struct dat_DDV __cw;
        unsigned int _cx;
        unsigned int _cy;
        struct dat_DDV _cz;
        _cg = iu19993__cc;
        _ch = w_param_d;
        __cf = _ch.data[_cg];
        _cj = __cf;
        _ck = 0;
        __ci.index = (_cj < _ck) ? 0 : 1;
        _cm = __ci;
        _cn.index = (_cm.index == 1) ? 0 : 1;
        __cl = _cn;
        _cp = __cl;
        _cq = __c6;
        __co.index = (!(_cq.index == _cp.index));
        _cs = __co;
         if ((_cs.index == 0))
         {
           __cr = 1;
         }
         else
         {
           __cr = 0;
         }
        _cu = iu19993__cc;
        _cv = doc_param_e;
        __ct = _cv.data[_cu];
        _cx = docUpdate_param_f;
        _cy = __ct;
        __cw.index = (_cx == _cy) ? 0 : 1;
        _cz = __cw;
         if ((_cz.index == 0))
         {
           _ce = __cr;
         }
         else
         {
           _ce = 0;
         }
        acc_cd += _ce;
      }
      __c9 = acc_cd;
      /* ----------- End Summate ----------- */
      lo_d0 = 0;
      hi_d1 = __c9;
      /* ---------- Begin Product ---------- */
      t_d2 = 0.0;
      c_d3 = 0.0;
      for (iu20003__d4 = lo_d0; iu20003__d4 < hi_d1; iu20003__d4++)
      {
        double x_d5;
        double y_d6;
        double z_d7;
        double __d8;
        unsigned int _d9;
        double p_da;
        double _db;
        double _dc;
        double _dd;
        _d9 = iu20003__d4;
        p_da = log(_d9);
        __d8 = p_da;
        _db = __c3;
        _dc = __d8;
        _dd = __a2;
        x_d5 = logSumExp3(_db,_dc,_dd);
        y_d6 = (x_d5 - c_d3);
        z_d7 = (t_d2 + y_d6);
        c_d3 = ((z_d7 - t_d2) - y_d6);
        t_d2 = z_d7;
      }
      x_b3 = t_d2;
      /* ----------- End Product ----------- */
      y_b4 = (x_b3 - c_b1);
      z_b5 = (t_b0 + y_b4);
      c_b1 = ((z_b5 - t_b0) - y_b4);
      t_b0 = z_b5;
    }
    __ax = t_b0;
    /* ----------- End Product ----------- */
    _df = __ax;
    __de = (-_df);
    _dh = zNewu19983__aw;
    _di = topic_prior_param_a;
    __dg = _di.data[_dh];
    lo_dk = 0;
    hi_dl = __d;
    /* ---------- Begin Summate ---------- */
    acc_dn = 0;
    for (iu19993__dm = lo_dk; iu19993__dm < hi_dl; iu19993__dm++)
    {
      unsigned int _do;
      unsigned int __dp;
      unsigned int _dq;
      struct array_nat _dr;
      struct dat_DDV __ds;
      unsigned int _dt;
      unsigned int _du;
      unsigned int __dv;
      struct dat_DDV _dw;
      struct dat_DDV __dx;
      unsigned int _dy;
      unsigned int _dz;
      struct dat_DDV _e0;
      _dq = iu19993__dm;
      _dr = z_param_c;
      __dp = _dr.data[_dq];
      _dt = zNewu19983__aw;
      _du = __dp;
      __ds.index = (_dt == _du) ? 0 : 1;
      _dw = __ds;
       if ((_dw.index == 0))
       {
         __dv = 1;
       }
       else
       {
         __dv = 0;
       }
      _dy = iu19993__dm;
      _dz = docUpdate_param_f;
      __dx.index = (_dy == _dz) ? 0 : 1;
      _e0 = __dx;
       if ((_e0.index == 0))
       {
         _do = 0;
       }
       else
       {
         _do = __dv;
       }
      acc_dn += _do;
    }
    __dj = acc_dn;
    /* ----------- End Summate ----------- */
    _e2 = __dj;
    p_e3 = log(_e2);
    __e1 = p_e3;
    _e5 = __e1;
    _e6 = __dg;
    __e4 = logSumExp2(_e5,_e6);
    lo_e8 = 0;
    hi_e9 = __ad;
    /* ---------- Begin Product ---------- */
    t_ea = 0.0;
    c_eb = 0.0;
    for (i__ec = lo_e8; i__ec < hi_e9; i__ec++)
    {
      double x_ed;
      double y_ee;
      double z_ef;
      struct dat_DDV __eg;
      unsigned int _eh;
      unsigned int _ei;
      unsigned int lo_ej;
      unsigned int hi_ek;
      unsigned int iu20003__en;
      double t_el;
      double c_em;
      _eh = i__ec;
      _ei = zNewu19983__aw;
      __eg.index = (_eh == _ei) ? 0 : 1;
      lo_ej = 0;
      hi_ek = __a0;
      /* ---------- Begin Product ---------- */
      t_el = 0.0;
      c_em = 0.0;
      for (iu20003__en = lo_ej; iu20003__en < hi_ek; iu20003__en++)
      {
        double x_eo;
        double y_ep;
        double z_eq;
        double __er;
        unsigned int _es;
        struct array_prob _et;
        unsigned int __eu;
        unsigned int lo_ev;
        unsigned int hi_ew;
        unsigned int iu19993__ex;
        unsigned int acc_ey;
        double __fo;
        unsigned int _fp;
        double p_fq;
        unsigned int __fr;
        unsigned int lo_fs;
        unsigned int hi_ft;
        unsigned int iu19993__fu;
        unsigned int acc_fv;
        unsigned int lo_gf;
        unsigned int hi_gg;
        unsigned int j__gj;
        double t_gh;
        double c_gi;
        _es = iu20003__en;
        _et = word_prior_param_b;
        __er = _et.data[_es];
        lo_ev = 0;
        hi_ew = __b;
        /* ---------- Begin Summate ---------- */
        acc_ey = 0;
        for (iu19993__ex = lo_ev; iu19993__ex < hi_ew; iu19993__ex++)
        {
          unsigned int _ez;
          unsigned int __f0;
          unsigned int _f1;
          struct array_nat _f2;
          struct dat_DDV __f3;
          unsigned int _f4;
          unsigned int _f5;
          unsigned int __f6;
          unsigned int _f7;
          struct array_nat _f8;
          unsigned int __f9;
          unsigned int _fa;
          struct array_nat _fb;
          struct dat_DDV __fc;
          unsigned int _fd;
          unsigned int _fe;
          struct dat_DDV __ff;
          struct dat_DDV _fg;
          struct dat_DDV _fh;
          unsigned int __fi;
          struct dat_DDV _fj;
          struct dat_DDV __fk;
          unsigned int _fl;
          unsigned int _fm;
          struct dat_DDV _fn;
          _f1 = iu19993__ex;
          _f2 = w_param_d;
          __f0 = _f2.data[_f1];
          _f4 = iu20003__en;
          _f5 = __f0;
          __f3.index = (_f4 == _f5) ? 0 : 1;
          _f7 = iu19993__ex;
          _f8 = doc_param_e;
          __f6 = _f8.data[_f7];
          _fa = __f6;
          _fb = z_param_c;
          __f9 = _fb.data[_fa];
          _fd = i__ec;
          _fe = __f9;
          __fc.index = (_fd == _fe) ? 0 : 1;
          _fg = __fc;
          _fh = __f3;
          __ff.index = (!(_fh.index == _fg.index));
          _fj = __ff;
           if ((_fj.index == 0))
           {
             __fi = 1;
           }
           else
           {
             __fi = 0;
           }
          _fl = __f6;
          _fm = docUpdate_param_f;
          __fk.index = (_fl == _fm) ? 0 : 1;
          _fn = __fk;
           if ((_fn.index == 0))
           {
             _ez = 0;
           }
           else
           {
             _ez = __fi;
           }
          acc_ey += _ez;
        }
        __eu = acc_ey;
        /* ----------- End Summate ----------- */
        _fp = __eu;
        p_fq = log(_fp);
        __fo = p_fq;
        lo_fs = 0;
        hi_ft = __b;
        /* ---------- Begin Summate ---------- */
        acc_fv = 0;
        for (iu19993__fu = lo_fs; iu19993__fu < hi_ft; iu19993__fu++)
        {
          unsigned int _fw;
          unsigned int __fx;
          unsigned int _fy;
          struct array_nat _fz;
          struct dat_DDV __g0;
          unsigned int _g1;
          unsigned int _g2;
          struct dat_DDV __g3;
          struct dat_DDV _g4;
          struct dat_DDV _g5;
          unsigned int __g6;
          struct dat_DDV _g7;
          unsigned int __g8;
          unsigned int _g9;
          struct array_nat _ga;
          struct dat_DDV __gb;
          unsigned int _gc;
          unsigned int _gd;
          struct dat_DDV _ge;
          _fy = iu19993__fu;
          _fz = w_param_d;
          __fx = _fz.data[_fy];
          _g1 = iu20003__en;
          _g2 = __fx;
          __g0.index = (_g1 == _g2) ? 0 : 1;
          _g4 = __eg;
          _g5 = __g0;
          __g3.index = (!(_g5.index == _g4.index));
          _g7 = __g3;
           if ((_g7.index == 0))
           {
             __g6 = 1;
           }
           else
           {
             __g6 = 0;
           }
          _g9 = iu19993__fu;
          _ga = doc_param_e;
          __g8 = _ga.data[_g9];
          _gc = docUpdate_param_f;
          _gd = __g8;
          __gb.index = (_gc == _gd) ? 0 : 1;
          _ge = __gb;
           if ((_ge.index == 0))
           {
             _fw = __g6;
           }
           else
           {
             _fw = 0;
           }
          acc_fv += _fw;
        }
        __fr = acc_fv;
        /* ----------- End Summate ----------- */
        lo_gf = 0;
        hi_gg = __fr;
        /* ---------- Begin Product ---------- */
        t_gh = 0.0;
        c_gi = 0.0;
        for (j__gj = lo_gf; j__gj < hi_gg; j__gj++)
        {
          double x_gk;
          double y_gl;
          double z_gm;
          double __gn;
          unsigned int _go;
          double p_gp;
          double _gq;
          double _gr;
          double _gs;
          _go = j__gj;
          p_gp = log(_go);
          __gn = p_gp;
          _gq = __fo;
          _gr = __gn;
          _gs = __er;
          x_gk = logSumExp3(_gq,_gr,_gs);
          y_gl = (x_gk - c_gi);
          z_gm = (t_gh + y_gl);
          c_gi = ((z_gm - t_gh) - y_gl);
          t_gh = z_gm;
        }
        x_eo = t_gh;
        /* ----------- End Product ----------- */
        y_ep = (x_eo - c_em);
        z_eq = (t_el + y_ep);
        c_em = ((z_eq - t_el) - y_ep);
        t_el = z_eq;
      }
      x_ed = t_el;
      /* ----------- End Product ----------- */
      y_ee = (x_ed - c_eb);
      z_ef = (t_ea + y_ee);
      c_eb = ((z_ef - t_ea) - y_ee);
      t_ea = z_ef;
    }
    __e7 = t_ea;
    /* ----------- End Product ----------- */
    _gt = __e7;
    _gu = __e4;
    _gv = __at;
    _gw = __de;
    __av.data[zNewu19983__aw] = (_gu + (_gv + (_gw + _gt)));
  }
  /* ------------ End Array ------------ */
  _gx = __av;
  ws_gz = log(0);
  max_h0 = log(0.0);
  for (it_gy = 0; it_gy < _gx.size; it_gy++)
  {
    if ((max_h0 < _gx.data[it_gy]))
    {
      max_h0 = _gx.data[it_gy];
    }
    ws_gz = logSumExp2(ws_gz,_gx.data[it_gy]);
  }
  ws_gz = (ws_gz - max_h0);
  r_h1 = (log((((double)rand()) / ((double)RAND_MAX))) + ws_gz);
  ws_gz = log(0);
  it_gy = 0;
  while (1)
  {
    if ((r_h1 < ws_gz))
    {
      out_a.weight = 0;
      out_a.sample = (it_gy - 1);
      break;
    }
    ws_gz = logSumExp2(ws_gz,(_gx.data[it_gy] - max_h0));
    it_gy++;
  }
  return out_a;
}

