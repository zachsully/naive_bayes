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

double logSumExp2(double _a, double _b)
{
  return (_a > _b) ? (_a + log1p((expm1((_b - _a)) + 1))) : (_b + log1p((expm1((_a - _b)) + 1)));
}

double logSumExp3(double _a, double _b, double _c)
{
  return (_a > _b) ? (_a > _c) ? (_a + log1p(((expm1((_c - _a)) + expm1((_b - _a))) + 2))) : (_c + log1p(((expm1((_b - _c)) + expm1((_a - _c))) + 2))) : (_b > _c) ? (_b + log1p(((expm1((_c - _b)) + expm1((_a - _b))) + 2))) : (_c + log1p(((expm1((_b - _c)) + expm1((_a - _c))) + 2)));
}

struct mdata_nat gibbsC(struct array_prob topic_prior_a, struct array_prob word_prior_b, struct array_nat z_c, struct array_nat w_d, struct array_nat doc_e, unsigned int docUpdate_f)
{
  struct mdata_nat out_g;
  struct dat_DDV _h;
  unsigned int _i;
  unsigned int _j;
  struct array_nat arr_k;
  struct mdata_nat _l;
  double _m;
  double _n;
  unsigned int lo_o;
  unsigned int hi_p;
  struct array_prob arr_q;
  unsigned int i_t;
  double t_r;
  double c_s;
  double _b8;
  unsigned int lo_b9;
  unsigned int hi_ba;
  struct array_prob arr_bb;
  unsigned int i_be;
  double t_bc;
  double c_bd;
  double _ca;
  double _cb;
  unsigned int lo_cc;
  unsigned int hi_cd;
  unsigned int lo_ce;
  unsigned int hi_cf;
  struct array_nat arr_cg;
  unsigned int iu19993_ch;
  unsigned int acc_ci;
  unsigned int i_cu;
  double t_cs;
  double c_ct;
  double _dd;
  double _de;
  unsigned int lo_df;
  unsigned int hi_dg;
  struct array_prob arr_dh;
  unsigned int i_dk;
  double t_di;
  double c_dj;
  struct array_prob _f0;
  struct array_prob arr_f1;
  unsigned int zNewu19983_f2;
  int it_l2;
  double ws_l3;
  double max_l4;
  double r_l5;
  _i = docUpdate_f;
  arr_k = z_c;
  _j = arr_k.size;
  _h.index = (_i < _j) ? 0 : 1;
   if ((_h.index == 0))
   {
     lo_o = 0;
     arr_q = topic_prior_a;
     hi_p = arr_q.size;
     /* ---------- Begin Product ---------- */
     t_r = 0.0;
     c_s = 0.0;
     for (i_t = lo_o; i_t < hi_p; i_t++)
     {
       double x_u;
       double y_v;
       double z_w;
       unsigned int lo_x;
       unsigned int hi_y;
       struct array_prob arr_z;
       unsigned int iu20003_a2;
       double t_a0;
       double c_a1;
       lo_x = 0;
       arr_z = word_prior_b;
       hi_y = arr_z.size;
       /* ---------- Begin Product ---------- */
       t_a0 = 0.0;
       c_a1 = 0.0;
       for (iu20003_a2 = lo_x; iu20003_a2 < hi_y; iu20003_a2++)
       {
         double x_a3;
         double y_a4;
         double z_a5;
         unsigned int lo_a6;
         unsigned int hi_a7;
         unsigned int lo_a8;
         unsigned int hi_a9;
         struct array_nat arr_aa;
         unsigned int iu19993_ab;
         unsigned int acc_ac;
         unsigned int j_ay;
         double t_aw;
         double c_ax;
         lo_a6 = 0;
         lo_a8 = 0;
         arr_aa = w_d;
         hi_a9 = arr_aa.size;
         /* ---------- Begin Summate ---------- */
         acc_ac = 0;
         for (iu19993_ab = lo_a8; iu19993_ab < hi_a9; iu19993_ab++)
         {
           unsigned int _ad;
           struct dat_DDV _ae;
           unsigned int _af;
           unsigned int _ag;
           struct array_nat arr_ah;
           unsigned int _ai;
           struct dat_DDV _aj;
           struct dat_DDV _ak;
           unsigned int _al;
           unsigned int _am;
           struct array_nat arr_an;
           unsigned int _ao;
           struct array_nat arr_ap;
           unsigned int _aq;
           struct dat_DDV _ar;
           unsigned int _as;
           unsigned int _at;
           struct array_nat arr_au;
           unsigned int _av;
           _af = docUpdate_f;
           arr_ah = doc_e;
           _ai = iu19993_ab;
           _ag = arr_ah.data[_ai];
           _ae.index = (_af == _ag) ? 0 : 1;
            if ((_ae.index == 0))
            {
              _ad = 0;
            }
            else
             if ((_ae.index == 1))
             {
               _al = i_t;
               arr_an = z_c;
               arr_ap = doc_e;
               _aq = iu19993_ab;
               _ao = arr_ap.data[_aq];
               _am = arr_an.data[_ao];
               _ak.index = (_al == _am) ? 0 : 1;
               _as = iu20003_a2;
               arr_au = w_d;
               _av = iu19993_ab;
               _at = arr_au.data[_av];
               _ar.index = (_as == _at) ? 0 : 1;
               _aj.index = (!(_ar.index == _ak.index));
                if ((_aj.index == 0))
                {
                  _ad = 1;
                }
                else
                 if ((_aj.index == 1))
                 {
                   _ad = 0;
                 }
             }
           acc_ac += _ad;
         }
         hi_a7 = acc_ac;
         /* ----------- End Summate ----------- */
         /* ---------- Begin Product ---------- */
         t_aw = 0.0;
         c_ax = 0.0;
         for (j_ay = lo_a6; j_ay < hi_a7; j_ay++)
         {
           double x_az;
           double y_b0;
           double z_b1;
           double _b2;
           unsigned int _b3;
           double p_b4;
           double _b5;
           struct array_prob arr_b6;
           unsigned int _b7;
           _b3 = j_ay;
           p_b4 = log1p((_b3 - 1));
           _b2 = p_b4;
           arr_b6 = word_prior_b;
           _b7 = iu20003_a2;
           _b5 = arr_b6.data[_b7];
           x_az = logSumExp2(_b2,_b5);
           y_b0 = (x_az - c_ax);
           z_b1 = (t_aw + y_b0);
           c_ax = ((z_b1 - t_aw) - y_b0);
           t_aw = z_b1;
         }
         x_a3 = t_aw;
         /* ----------- End Product ----------- */
         y_a4 = (x_a3 - c_a1);
         z_a5 = (t_a0 + y_a4);
         c_a1 = ((z_a5 - t_a0) - y_a4);
         t_a0 = z_a5;
       }
       x_u = t_a0;
       /* ----------- End Product ----------- */
       y_v = (x_u - c_s);
       z_w = (t_r + y_v);
       c_s = ((z_w - t_r) - y_v);
       t_r = z_w;
     }
     _n = t_r;
     /* ----------- End Product ----------- */
     lo_b9 = 0;
     arr_bb = topic_prior_a;
     hi_ba = arr_bb.size;
     /* ---------- Begin Product ---------- */
     t_bc = 0.0;
     c_bd = 0.0;
     for (i_be = lo_b9; i_be < hi_ba; i_be++)
     {
       double x_bf;
       double y_bg;
       double z_bh;
       unsigned int lo_bi;
       unsigned int hi_bj;
       unsigned int lo_bk;
       unsigned int hi_bl;
       struct array_nat arr_bm;
       unsigned int iu19993_bn;
       unsigned int acc_bo;
       unsigned int j_c0;
       double t_by;
       double c_bz;
       lo_bi = 0;
       lo_bk = 0;
       arr_bm = z_c;
       hi_bl = arr_bm.size;
       /* ---------- Begin Summate ---------- */
       acc_bo = 0;
       for (iu19993_bn = lo_bk; iu19993_bn < hi_bl; iu19993_bn++)
       {
         unsigned int _bp;
         struct dat_DDV _bq;
         unsigned int _br;
         unsigned int _bs;
         struct dat_DDV _bt;
         unsigned int _bu;
         unsigned int _bv;
         struct array_nat arr_bw;
         unsigned int _bx;
         _br = iu19993_bn;
         _bs = docUpdate_f;
         _bq.index = (_br == _bs) ? 0 : 1;
          if ((_bq.index == 0))
          {
            _bp = 0;
          }
          else
           if ((_bq.index == 1))
           {
             _bu = i_be;
             arr_bw = z_c;
             _bx = iu19993_bn;
             _bv = arr_bw.data[_bx];
             _bt.index = (_bu == _bv) ? 0 : 1;
              if ((_bt.index == 0))
              {
                _bp = 1;
              }
              else
               if ((_bt.index == 1))
               {
                 _bp = 0;
               }
           }
         acc_bo += _bp;
       }
       hi_bj = acc_bo;
       /* ----------- End Summate ----------- */
       /* ---------- Begin Product ---------- */
       t_by = 0.0;
       c_bz = 0.0;
       for (j_c0 = lo_bi; j_c0 < hi_bj; j_c0++)
       {
         double x_c1;
         double y_c2;
         double z_c3;
         double _c4;
         unsigned int _c5;
         double p_c6;
         double _c7;
         struct array_prob arr_c8;
         unsigned int _c9;
         _c5 = j_c0;
         p_c6 = log1p((_c5 - 1));
         _c4 = p_c6;
         arr_c8 = topic_prior_a;
         _c9 = i_be;
         _c7 = arr_c8.data[_c9];
         x_c1 = logSumExp2(_c4,_c7);
         y_c2 = (x_c1 - c_bz);
         z_c3 = (t_by + y_c2);
         c_bz = ((z_c3 - t_by) - y_c2);
         t_by = z_c3;
       }
       x_bf = t_by;
       /* ----------- End Product ----------- */
       y_bg = (x_bf - c_bd);
       z_bh = (t_bc + y_bg);
       c_bd = ((z_bh - t_bc) - y_bg);
       t_bc = z_bh;
     }
     _b8 = t_bc;
     /* ----------- End Product ----------- */
     lo_cc = 0;
     lo_ce = 0;
     arr_cg = z_c;
     hi_cf = arr_cg.size;
     /* ---------- Begin Summate ---------- */
     acc_ci = 0;
     for (iu19993_ch = lo_ce; iu19993_ch < hi_cf; iu19993_ch++)
     {
       unsigned int _cj;
       struct dat_DDV _ck;
       unsigned int _cl;
       unsigned int _cm;
       struct dat_DDV _cn;
       unsigned int _co;
       unsigned int _cp;
       struct array_nat arr_cq;
       unsigned int _cr;
       _cl = iu19993_ch;
       _cm = docUpdate_f;
       _ck.index = (_cl == _cm) ? 0 : 1;
        if ((_ck.index == 0))
        {
          _cj = 0;
        }
        else
         if ((_ck.index == 1))
         {
           arr_cq = z_c;
           _cr = iu19993_ch;
           _co = arr_cq.data[_cr];
           _cp = 0;
           _cn.index = (_co < _cp) ? 0 : 1;
            if ((_cn.index == 0))
            {
              _cj = 0;
            }
            else
             if ((_cn.index == 1))
             {
               _cj = 1;
             }
         }
       acc_ci += _cj;
     }
     hi_cd = acc_ci;
     /* ----------- End Summate ----------- */
     /* ---------- Begin Product ---------- */
     t_cs = 0.0;
     c_ct = 0.0;
     for (i_cu = lo_cc; i_cu < hi_cd; i_cu++)
     {
       double x_cv;
       double y_cw;
       double z_cx;
       double _cy;
       unsigned int _cz;
       double p_d0;
       double _d1;
       unsigned int lo_d2;
       unsigned int hi_d3;
       struct array_prob arr_d4;
       struct array_prob summate_arr_d5;
       double maxV_d6;
       double sum_d8;
       unsigned int maxI_d7;
       unsigned int iu19993_d9;
       _cz = i_cu;
       p_d0 = log1p((_cz - 1));
       _cy = p_d0;
       lo_d2 = 0;
       arr_d4 = topic_prior_a;
       hi_d3 = arr_d4.size;
       /* ---------- Begin Summate ---------- */
       summate_arr_d5.size = (hi_d3 - lo_d2);
       summate_arr_d5.data = ((double *)malloc((summate_arr_d5.size * sizeof(double))));
       for (iu19993_d9 = 0; iu19993_d9 < summate_arr_d5.size; iu19993_d9++)
       {
         double _da;
         struct array_prob arr_db;
         unsigned int _dc;
         arr_db = topic_prior_a;
         _dc = iu19993_d9;
         _da = arr_db.data[_dc];
         summate_arr_d5.data[iu19993_d9] = _da;
         if (((maxV_d6 < _da) || (iu19993_d9 == 0)))
         {
           maxV_d6 = _da;
           maxI_d7 = iu19993_d9;
         }
       }
       sum_d8 = 0.0;
       for (iu19993_d9 = 0; iu19993_d9 < summate_arr_d5.size; iu19993_d9++)
       {
         if ((iu19993_d9 != maxI_d7))
          sum_d8 += exp((summate_arr_d5.data[iu19993_d9] - maxV_d6));
       }
       _d1 = (maxV_d6 + log(sum_d8));
       free(summate_arr_d5.data);
       /* ----------- End Summate ----------- */
       x_cv = logSumExp2(_cy,_d1);
       y_cw = (x_cv - c_ct);
       z_cx = (t_cs + y_cw);
       c_ct = ((z_cx - t_cs) - y_cw);
       t_cs = z_cx;
     }
     _cb = t_cs;
     /* ----------- End Product ----------- */
     _ca = (-_cb);
     lo_df = 0;
     arr_dh = topic_prior_a;
     hi_dg = arr_dh.size;
     /* ---------- Begin Product ---------- */
     t_di = 0.0;
     c_dj = 0.0;
     for (i_dk = lo_df; i_dk < hi_dg; i_dk++)
     {
       double x_dl;
       double y_dm;
       double z_dn;
       unsigned int lo_do;
       unsigned int hi_dp;
       unsigned int lo_dq;
       unsigned int hi_dr;
       struct array_nat arr_ds;
       unsigned int iu19993_dt;
       unsigned int acc_du;
       unsigned int iu20003_eh;
       double t_ef;
       double c_eg;
       lo_do = 0;
       lo_dq = 0;
       arr_ds = w_d;
       hi_dr = arr_ds.size;
       /* ---------- Begin Summate ---------- */
       acc_du = 0;
       for (iu19993_dt = lo_dq; iu19993_dt < hi_dr; iu19993_dt++)
       {
         unsigned int _dv;
         struct dat_DDV _dw;
         unsigned int _dx;
         unsigned int _dy;
         struct array_nat arr_dz;
         unsigned int _e0;
         struct dat_DDV _e1;
         struct dat_DDV _e2;
         struct dat_DDV not_e3;
         unsigned int _e4;
         unsigned int _e5;
         struct array_nat arr_e6;
         unsigned int _e7;
         struct dat_DDV _e8;
         unsigned int _e9;
         unsigned int _ea;
         struct array_nat arr_eb;
         unsigned int _ec;
         struct array_nat arr_ed;
         unsigned int _ee;
         _dx = docUpdate_f;
         arr_dz = doc_e;
         _e0 = iu19993_dt;
         _dy = arr_dz.data[_e0];
         _dw.index = (_dx == _dy) ? 0 : 1;
          if ((_dw.index == 0))
          {
            _dv = 0;
          }
          else
           if ((_dw.index == 1))
           {
             arr_e6 = w_d;
             _e7 = iu19993_dt;
             _e4 = arr_e6.data[_e7];
             _e5 = 0;
             not_e3.index = (_e4 < _e5) ? 0 : 1;
             not_e3.index = (not_e3.index == 1) ? 0 : 1;
             _e9 = i_dk;
             arr_eb = z_c;
             arr_ed = doc_e;
             _ee = iu19993_dt;
             _ec = arr_ed.data[_ee];
             _ea = arr_eb.data[_ec];
             _e8.index = (_e9 == _ea) ? 0 : 1;
             _e1.index = (!(_e8.index == _e2.index));
              if ((_e1.index == 0))
              {
                _dv = 1;
              }
              else
               if ((_e1.index == 1))
               {
                 _dv = 0;
               }
           }
         acc_du += _dv;
       }
       hi_dp = acc_du;
       /* ----------- End Summate ----------- */
       /* ---------- Begin Product ---------- */
       t_ef = 0.0;
       c_eg = 0.0;
       for (iu20003_eh = lo_do; iu20003_eh < hi_dp; iu20003_eh++)
       {
         double x_ei;
         double y_ej;
         double z_ek;
         double _el;
         unsigned int _em;
         double p_en;
         double _eo;
         unsigned int lo_ep;
         unsigned int hi_eq;
         struct array_prob arr_er;
         struct array_prob summate_arr_es;
         double maxV_et;
         double sum_ev;
         unsigned int maxI_eu;
         unsigned int iu19993_ew;
         _em = iu20003_eh;
         p_en = log1p((_em - 1));
         _el = p_en;
         lo_ep = 0;
         arr_er = word_prior_b;
         hi_eq = arr_er.size;
         /* ---------- Begin Summate ---------- */
         summate_arr_es.size = (hi_eq - lo_ep);
         summate_arr_es.data = ((double *)malloc((summate_arr_es.size * sizeof(double))));
         for (iu19993_ew = 0; iu19993_ew < summate_arr_es.size; iu19993_ew++)
         {
           double _ex;
           struct array_prob arr_ey;
           unsigned int _ez;
           arr_ey = word_prior_b;
           _ez = iu19993_ew;
           _ex = arr_ey.data[_ez];
           summate_arr_es.data[iu19993_ew] = _ex;
           if (((maxV_et < _ex) || (iu19993_ew == 0)))
           {
             maxV_et = _ex;
             maxI_eu = iu19993_ew;
           }
         }
         sum_ev = 0.0;
         for (iu19993_ew = 0; iu19993_ew < summate_arr_es.size; iu19993_ew++)
         {
           if ((iu19993_ew != maxI_eu))
            sum_ev += exp((summate_arr_es.data[iu19993_ew] - maxV_et));
         }
         _eo = (maxV_et + log(sum_ev));
         free(summate_arr_es.data);
         /* ----------- End Summate ----------- */
         x_ei = logSumExp2(_el,_eo);
         y_ej = (x_ei - c_eg);
         z_ek = (t_ef + y_ej);
         c_eg = ((z_ek - t_ef) - y_ej);
         t_ef = z_ek;
       }
       x_dl = t_ef;
       /* ----------- End Product ----------- */
       y_dm = (x_dl - c_dj);
       z_dn = (t_di + y_dm);
       c_dj = ((z_dn - t_di) - y_dm);
       t_di = z_dn;
     }
     _de = t_di;
     /* ----------- End Product ----------- */
     _dd = (-_de);
     _m = (_b8 + (_ca + (_dd + _n)));
     arr_f1 = topic_prior_a;
     _f0.size = arr_f1.size;
     _f0.data = ((double *)malloc((_f0.size * sizeof(double))));
     /* ----------- Begin Array ----------- */
     for (zNewu19983_f2 = 0; zNewu19983_f2 < _f0.size; zNewu19983_f2++)
     {
       double _f3;
       unsigned int lo_f4;
       unsigned int hi_f5;
       struct array_prob arr_f6;
       unsigned int i_f9;
       double t_f7;
       double c_f8;
       double _hb;
       double _hc;
       unsigned int _hd;
       unsigned int lo_he;
       unsigned int hi_hf;
       struct array_nat arr_hg;
       unsigned int iu19993_hh;
       unsigned int acc_hi;
       double p_hs;
       double _ht;
       struct array_prob arr_hu;
       unsigned int _hv;
       double _hw;
       double _hx;
       double _hy;
       unsigned int _hz;
       unsigned int lo_i0;
       unsigned int hi_i1;
       struct array_nat arr_i2;
       unsigned int iu19993_i3;
       unsigned int acc_i4;
       double p_ie;
       double _if;
       unsigned int lo_ig;
       unsigned int hi_ih;
       struct array_prob arr_ii;
       struct array_prob summate_arr_ij;
       double maxV_ik;
       double sum_im;
       unsigned int maxI_il;
       unsigned int iu19993_in;
       double _ir;
       double _is;
       unsigned int lo_it;
       unsigned int hi_iu;
       struct array_prob arr_iv;
       unsigned int i_iy;
       double t_iw;
       double c_ix;
       lo_f4 = 0;
       arr_f6 = topic_prior_a;
       hi_f5 = arr_f6.size;
       /* ---------- Begin Product ---------- */
       t_f7 = 0.0;
       c_f8 = 0.0;
       for (i_f9 = lo_f4; i_f9 < hi_f5; i_f9++)
       {
         double x_fa;
         double y_fb;
         double z_fc;
         unsigned int lo_fd;
         unsigned int hi_fe;
         struct array_prob arr_ff;
         unsigned int iu20003_fi;
         double t_fg;
         double c_fh;
         lo_fd = 0;
         arr_ff = word_prior_b;
         hi_fe = arr_ff.size;
         /* ---------- Begin Product ---------- */
         t_fg = 0.0;
         c_fh = 0.0;
         for (iu20003_fi = lo_fd; iu20003_fi < hi_fe; iu20003_fi++)
         {
           double x_fj;
           double y_fk;
           double z_fl;
           unsigned int lo_fm;
           unsigned int hi_fn;
           unsigned int lo_fo;
           unsigned int hi_fp;
           struct array_nat arr_fq;
           unsigned int iu19993_fr;
           unsigned int acc_fs;
           unsigned int j_ga;
           double t_g8;
           double c_g9;
           lo_fm = 0;
           lo_fo = 0;
           arr_fq = w_d;
           hi_fp = arr_fq.size;
           /* ---------- Begin Summate ---------- */
           acc_fs = 0;
           for (iu19993_fr = lo_fo; iu19993_fr < hi_fp; iu19993_fr++)
           {
             unsigned int _ft;
             struct dat_DDV _fu;
             unsigned int _fv;
             unsigned int _fw;
             struct array_nat arr_fx;
             unsigned int _fy;
             struct dat_DDV _fz;
             struct dat_DDV _g0;
             unsigned int _g1;
             unsigned int _g2;
             struct dat_DDV _g3;
             unsigned int _g4;
             unsigned int _g5;
             struct array_nat arr_g6;
             unsigned int _g7;
             _fv = docUpdate_f;
             arr_fx = doc_e;
             _fy = iu19993_fr;
             _fw = arr_fx.data[_fy];
             _fu.index = (_fv == _fw) ? 0 : 1;
              if ((_fu.index == 0))
              {
                _g1 = i_f9;
                _g2 = zNewu19983_f2;
                _g0.index = (_g1 == _g2) ? 0 : 1;
                _g4 = iu20003_fi;
                arr_g6 = w_d;
                _g7 = iu19993_fr;
                _g5 = arr_g6.data[_g7];
                _g3.index = (_g4 == _g5) ? 0 : 1;
                _fz.index = (!(_g3.index == _g0.index));
                 if ((_fz.index == 0))
                 {
                   _ft = 1;
                 }
                 else
                  if ((_fz.index == 1))
                  {
                    _ft = 0;
                  }
              }
              else
               if ((_fu.index == 1))
               {
                 _ft = 0;
               }
             acc_fs += _ft;
           }
           hi_fn = acc_fs;
           /* ----------- End Summate ----------- */
           /* ---------- Begin Product ---------- */
           t_g8 = 0.0;
           c_g9 = 0.0;
           for (j_ga = lo_fm; j_ga < hi_fn; j_ga++)
           {
             double x_gb;
             double y_gc;
             double z_gd;
             double _ge;
             unsigned int _gf;
             unsigned int lo_gg;
             unsigned int hi_gh;
             struct array_nat arr_gi;
             unsigned int iu19993_gj;
             unsigned int acc_gk;
             double p_h4;
             double _h5;
             unsigned int _h6;
             double p_h7;
             double _h8;
             struct array_prob arr_h9;
             unsigned int _ha;
             lo_gg = 0;
             arr_gi = w_d;
             hi_gh = arr_gi.size;
             /* ---------- Begin Summate ---------- */
             acc_gk = 0;
             for (iu19993_gj = lo_gg; iu19993_gj < hi_gh; iu19993_gj++)
             {
               unsigned int _gl;
               struct dat_DDV _gm;
               unsigned int _gn;
               unsigned int _go;
               struct array_nat arr_gp;
               unsigned int _gq;
               struct dat_DDV _gr;
               struct dat_DDV _gs;
               unsigned int _gt;
               unsigned int _gu;
               struct array_nat arr_gv;
               unsigned int _gw;
               struct array_nat arr_gx;
               unsigned int _gy;
               struct dat_DDV _gz;
               unsigned int _h0;
               unsigned int _h1;
               struct array_nat arr_h2;
               unsigned int _h3;
               arr_gp = doc_e;
               _gq = iu19993_gj;
               _gn = arr_gp.data[_gq];
               _go = docUpdate_f;
               _gm.index = (_gn == _go) ? 0 : 1;
                if ((_gm.index == 0))
                {
                  _gl = 0;
                }
                else
                 if ((_gm.index == 1))
                 {
                   _gt = i_f9;
                   arr_gv = z_c;
                   arr_gx = doc_e;
                   _gy = iu19993_gj;
                   _gw = arr_gx.data[_gy];
                   _gu = arr_gv.data[_gw];
                   _gs.index = (_gt == _gu) ? 0 : 1;
                   _h0 = iu20003_fi;
                   arr_h2 = w_d;
                   _h3 = iu19993_gj;
                   _h1 = arr_h2.data[_h3];
                   _gz.index = (_h0 == _h1) ? 0 : 1;
                   _gr.index = (!(_gz.index == _gs.index));
                    if ((_gr.index == 0))
                    {
                      _gl = 1;
                    }
                    else
                     if ((_gr.index == 1))
                     {
                       _gl = 0;
                     }
                 }
               acc_gk += _gl;
             }
             _gf = acc_gk;
             /* ----------- End Summate ----------- */
             p_h4 = log1p((_gf - 1));
             _ge = p_h4;
             _h6 = j_ga;
             p_h7 = log1p((_h6 - 1));
             _h5 = p_h7;
             arr_h9 = word_prior_b;
             _ha = iu20003_fi;
             _h8 = arr_h9.data[_ha];
             x_gb = logSumExp3(_ge,_h5,_h8);
             y_gc = (x_gb - c_g9);
             z_gd = (t_g8 + y_gc);
             c_g9 = ((z_gd - t_g8) - y_gc);
             t_g8 = z_gd;
           }
           x_fj = t_g8;
           /* ----------- End Product ----------- */
           y_fk = (x_fj - c_fh);
           z_fl = (t_fg + y_fk);
           c_fh = ((z_fl - t_fg) - y_fk);
           t_fg = z_fl;
         }
         x_fa = t_fg;
         /* ----------- End Product ----------- */
         y_fb = (x_fa - c_f8);
         z_fc = (t_f7 + y_fb);
         c_f8 = ((z_fc - t_f7) - y_fb);
         t_f7 = z_fc;
       }
       _f3 = t_f7;
       /* ----------- End Product ----------- */
       lo_he = 0;
       arr_hg = z_c;
       hi_hf = arr_hg.size;
       /* ---------- Begin Summate ---------- */
       acc_hi = 0;
       for (iu19993_hh = lo_he; iu19993_hh < hi_hf; iu19993_hh++)
       {
         unsigned int _hj;
         struct dat_DDV _hk;
         unsigned int _hl;
         unsigned int _hm;
         struct dat_DDV _hn;
         unsigned int _ho;
         unsigned int _hp;
         struct array_nat arr_hq;
         unsigned int _hr;
         _hl = iu19993_hh;
         _hm = docUpdate_f;
         _hk.index = (_hl == _hm) ? 0 : 1;
          if ((_hk.index == 0))
          {
            _hj = 0;
          }
          else
           if ((_hk.index == 1))
           {
             _ho = zNewu19983_f2;
             arr_hq = z_c;
             _hr = iu19993_hh;
             _hp = arr_hq.data[_hr];
             _hn.index = (_ho == _hp) ? 0 : 1;
              if ((_hn.index == 0))
              {
                _hj = 1;
              }
              else
               if ((_hn.index == 1))
               {
                 _hj = 0;
               }
           }
         acc_hi += _hj;
       }
       _hd = acc_hi;
       /* ----------- End Summate ----------- */
       p_hs = log1p((_hd - 1));
       _hc = p_hs;
       arr_hu = topic_prior_a;
       _hv = zNewu19983_f2;
       _ht = arr_hu.data[_hv];
       _hb = logSumExp2(_hc,_ht);
       lo_i0 = 0;
       arr_i2 = z_c;
       hi_i1 = arr_i2.size;
       /* ---------- Begin Summate ---------- */
       acc_i4 = 0;
       for (iu19993_i3 = lo_i0; iu19993_i3 < hi_i1; iu19993_i3++)
       {
         unsigned int _i5;
         struct dat_DDV _i6;
         unsigned int _i7;
         unsigned int _i8;
         struct dat_DDV _i9;
         unsigned int _ia;
         unsigned int _ib;
         struct array_nat arr_ic;
         unsigned int _id;
         _i7 = iu19993_i3;
         _i8 = docUpdate_f;
         _i6.index = (_i7 == _i8) ? 0 : 1;
          if ((_i6.index == 0))
          {
            _i5 = 0;
          }
          else
           if ((_i6.index == 1))
           {
             arr_ic = z_c;
             _id = iu19993_i3;
             _ia = arr_ic.data[_id];
             _ib = 0;
             _i9.index = (_ia < _ib) ? 0 : 1;
              if ((_i9.index == 0))
              {
                _i5 = 0;
              }
              else
               if ((_i9.index == 1))
               {
                 _i5 = 1;
               }
           }
         acc_i4 += _i5;
       }
       _hz = acc_i4;
       /* ----------- End Summate ----------- */
       p_ie = log1p((_hz - 1));
       _hy = p_ie;
       lo_ig = 0;
       arr_ii = topic_prior_a;
       hi_ih = arr_ii.size;
       /* ---------- Begin Summate ---------- */
       summate_arr_ij.size = (hi_ih - lo_ig);
       summate_arr_ij.data = ((double *)malloc((summate_arr_ij.size * sizeof(double))));
       for (iu19993_in = 0; iu19993_in < summate_arr_ij.size; iu19993_in++)
       {
         double _io;
         struct array_prob arr_ip;
         unsigned int _iq;
         arr_ip = topic_prior_a;
         _iq = iu19993_in;
         _io = arr_ip.data[_iq];
         summate_arr_ij.data[iu19993_in] = _io;
         if (((maxV_ik < _io) || (iu19993_in == 0)))
         {
           maxV_ik = _io;
           maxI_il = iu19993_in;
         }
       }
       sum_im = 0.0;
       for (iu19993_in = 0; iu19993_in < summate_arr_ij.size; iu19993_in++)
       {
         if ((iu19993_in != maxI_il))
          sum_im += exp((summate_arr_ij.data[iu19993_in] - maxV_ik));
       }
       _if = (maxV_ik + log(sum_im));
       free(summate_arr_ij.data);
       /* ----------- End Summate ----------- */
       _hx = logSumExp2(_hy,_if);
       _hw = (-_hx);
       lo_it = 0;
       arr_iv = topic_prior_a;
       hi_iu = arr_iv.size;
       /* ---------- Begin Product ---------- */
       t_iw = 0.0;
       c_ix = 0.0;
       for (i_iy = lo_it; i_iy < hi_iu; i_iy++)
       {
         double x_iz;
         double y_j0;
         double z_j1;
         unsigned int lo_j2;
         unsigned int hi_j3;
         unsigned int lo_j4;
         unsigned int hi_j5;
         struct array_nat arr_j6;
         unsigned int iu19993_j7;
         unsigned int acc_j8;
         unsigned int iu20003_jr;
         double t_jp;
         double c_jq;
         lo_j2 = 0;
         lo_j4 = 0;
         arr_j6 = w_d;
         hi_j5 = arr_j6.size;
         /* ---------- Begin Summate ---------- */
         acc_j8 = 0;
         for (iu19993_j7 = lo_j4; iu19993_j7 < hi_j5; iu19993_j7++)
         {
           unsigned int _j9;
           struct dat_DDV _ja;
           unsigned int _jb;
           unsigned int _jc;
           struct array_nat arr_jd;
           unsigned int _je;
           struct dat_DDV _jf;
           struct dat_DDV _jg;
           struct dat_DDV not_jh;
           unsigned int _ji;
           unsigned int _jj;
           struct array_nat arr_jk;
           unsigned int _jl;
           struct dat_DDV _jm;
           unsigned int _jn;
           unsigned int _jo;
           _jb = docUpdate_f;
           arr_jd = doc_e;
           _je = iu19993_j7;
           _jc = arr_jd.data[_je];
           _ja.index = (_jb == _jc) ? 0 : 1;
            if ((_ja.index == 0))
            {
              arr_jk = w_d;
              _jl = iu19993_j7;
              _ji = arr_jk.data[_jl];
              _jj = 0;
              not_jh.index = (_ji < _jj) ? 0 : 1;
              not_jh.index = (not_jh.index == 1) ? 0 : 1;
              _jn = i_iy;
              _jo = zNewu19983_f2;
              _jm.index = (_jn == _jo) ? 0 : 1;
              _jf.index = (!(_jm.index == _jg.index));
               if ((_jf.index == 0))
               {
                 _j9 = 1;
               }
               else
                if ((_jf.index == 1))
                {
                  _j9 = 0;
                }
            }
            else
             if ((_ja.index == 1))
             {
               _j9 = 0;
             }
           acc_j8 += _j9;
         }
         hi_j3 = acc_j8;
         /* ----------- End Summate ----------- */
         /* ---------- Begin Product ---------- */
         t_jp = 0.0;
         c_jq = 0.0;
         for (iu20003_jr = lo_j2; iu20003_jr < hi_j3; iu20003_jr++)
         {
           double x_js;
           double y_jt;
           double z_ju;
           double _jv;
           unsigned int _jw;
           unsigned int lo_jx;
           unsigned int hi_jy;
           struct array_nat arr_jz;
           unsigned int iu19993_k0;
           unsigned int acc_k1;
           double p_km;
           double _kn;
           unsigned int _ko;
           double p_kp;
           double _kq;
           unsigned int lo_kr;
           unsigned int hi_ks;
           struct array_prob arr_kt;
           struct array_prob summate_arr_ku;
           double maxV_kv;
           double sum_kx;
           unsigned int maxI_kw;
           unsigned int iu19993_ky;
           lo_jx = 0;
           arr_jz = w_d;
           hi_jy = arr_jz.size;
           /* ---------- Begin Summate ---------- */
           acc_k1 = 0;
           for (iu19993_k0 = lo_jx; iu19993_k0 < hi_jy; iu19993_k0++)
           {
             unsigned int _k2;
             struct dat_DDV _k3;
             unsigned int _k4;
             unsigned int _k5;
             struct array_nat arr_k6;
             unsigned int _k7;
             struct dat_DDV _k8;
             struct dat_DDV _k9;
             struct dat_DDV not_ka;
             unsigned int _kb;
             unsigned int _kc;
             struct array_nat arr_kd;
             unsigned int _ke;
             struct dat_DDV _kf;
             unsigned int _kg;
             unsigned int _kh;
             struct array_nat arr_ki;
             unsigned int _kj;
             struct array_nat arr_kk;
             unsigned int _kl;
             arr_k6 = doc_e;
             _k7 = iu19993_k0;
             _k4 = arr_k6.data[_k7];
             _k5 = docUpdate_f;
             _k3.index = (_k4 == _k5) ? 0 : 1;
              if ((_k3.index == 0))
              {
                _k2 = 0;
              }
              else
               if ((_k3.index == 1))
               {
                 arr_kd = w_d;
                 _ke = iu19993_k0;
                 _kb = arr_kd.data[_ke];
                 _kc = 0;
                 not_ka.index = (_kb < _kc) ? 0 : 1;
                 not_ka.index = (not_ka.index == 1) ? 0 : 1;
                 _kg = i_iy;
                 arr_ki = z_c;
                 arr_kk = doc_e;
                 _kl = iu19993_k0;
                 _kj = arr_kk.data[_kl];
                 _kh = arr_ki.data[_kj];
                 _kf.index = (_kg == _kh) ? 0 : 1;
                 _k8.index = (!(_kf.index == _k9.index));
                  if ((_k8.index == 0))
                  {
                    _k2 = 1;
                  }
                  else
                   if ((_k8.index == 1))
                   {
                     _k2 = 0;
                   }
               }
             acc_k1 += _k2;
           }
           _jw = acc_k1;
           /* ----------- End Summate ----------- */
           p_km = log1p((_jw - 1));
           _jv = p_km;
           _ko = iu20003_jr;
           p_kp = log1p((_ko - 1));
           _kn = p_kp;
           lo_kr = 0;
           arr_kt = word_prior_b;
           hi_ks = arr_kt.size;
           /* ---------- Begin Summate ---------- */
           summate_arr_ku.size = (hi_ks - lo_kr);
           summate_arr_ku.data = ((double *)malloc((summate_arr_ku.size * sizeof(double))));
           for (iu19993_ky = 0; iu19993_ky < summate_arr_ku.size; iu19993_ky++)
           {
             double _kz;
             struct array_prob arr_l0;
             unsigned int _l1;
             arr_l0 = word_prior_b;
             _l1 = iu19993_ky;
             _kz = arr_l0.data[_l1];
             summate_arr_ku.data[iu19993_ky] = _kz;
             if (((maxV_kv < _kz) || (iu19993_ky == 0)))
             {
               maxV_kv = _kz;
               maxI_kw = iu19993_ky;
             }
           }
           sum_kx = 0.0;
           for (iu19993_ky = 0; iu19993_ky < summate_arr_ku.size; iu19993_ky++)
           {
             if ((iu19993_ky != maxI_kw))
              sum_kx += exp((summate_arr_ku.data[iu19993_ky] - maxV_kv));
           }
           _kq = (maxV_kv + log(sum_kx));
           free(summate_arr_ku.data);
           /* ----------- End Summate ----------- */
           x_js = logSumExp3(_jv,_kn,_kq);
           y_jt = (x_js - c_jq);
           z_ju = (t_jp + y_jt);
           c_jq = ((z_ju - t_jp) - y_jt);
           t_jp = z_ju;
         }
         x_iz = t_jp;
         /* ----------- End Product ----------- */
         y_j0 = (x_iz - c_ix);
         z_j1 = (t_iw + y_j0);
         c_ix = ((z_j1 - t_iw) - y_j0);
         t_iw = z_j1;
       }
       _is = t_iw;
       /* ----------- End Product ----------- */
       _ir = (-_is);
       _f0.data[zNewu19983_f2] = (_hb + (_hw + (_ir + _f3)));
     }
     /* ------------ End Array ------------ */
     ws_l3 = log(0);
     max_l4 = log(0.0);
     for (it_l2 = 0; it_l2 < _f0.size; it_l2++)
     {
       if ((max_l4 < _f0.data[it_l2]))
       {
         max_l4 = _f0.data[it_l2];
       }
       ws_l3 = logSumExp2(ws_l3,_f0.data[it_l2]);
     }
     ws_l3 = (ws_l3 - max_l4);
     r_l5 = ((((double)rand()) / ((double)RAND_MAX)) * exp(ws_l3));
     ws_l3 = log(0);
     it_l2 = 0;
     while (1)
     {
       if ((r_l5 < exp(ws_l3)))
       {
         _l.weight = 0;
         _l.sample = (it_l2 - 1);
         break;
       }
       ws_l3 = logSumExp2(ws_l3,(_f0.data[it_l2] - max_l4));
       it_l2++;
     }
     out_g.weight = (_l.weight + _m);
     out_g.sample = _l.sample;
   }
   else
    if ((_h.index == 1))
    {
      out_g.weight = 0;
    }
  return out_g;
}

