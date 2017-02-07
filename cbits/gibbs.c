#include "nb_simp.c"

struct arrayProb* gibbsC_shim(struct arrayProb* topic_prior_b,
                struct arrayProb* word_prior_c,
                struct arrayNat* z_d,
                struct arrayNat* w_e,
                struct arrayNat* doc_f,
                unsigned int docUpdate_g)
{
  struct arrayProb* res = (struct arrayProb*)malloc(sizeof(struct arrayProb));
  *res = gibbsC(*topic_prior_b, *word_prior_c, *z_d, *w_e, *doc_f, docUpdate_g);
  return res;
}

