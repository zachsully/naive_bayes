#include "nb_simp.c"

struct mdata_nat* gibbsC_shim(
        struct array_prob* topic_prior_b,
        struct array_prob* word_prior_c,
        struct array_nat* z_d,
        struct array_nat* w_e,
        struct array_nat* doc_f,
        unsigned int docUpdate_g)
{
  struct mdata_nat* res = (struct mdata_nat*)malloc(sizeof(struct mdata_nat));
  *res = gibbsC(*topic_prior_b, *word_prior_c, *z_d, *w_e, *doc_f, docUpdate_g);
  return res;
}
