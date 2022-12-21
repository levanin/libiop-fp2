import csv
from math import ceil
int_from_string = lambda x: int("".join([c for c in x if c.isdigit()]))
limbs = lambda x: ceil(int_from_string(x)/64)
contains_plus = lambda x: any([c == '+' for c in x])
get_n_m_p = lambda x: [11*x+4, 11*x + 2]
get_n_m_p2 = lambda x: [4*x+4, 4*x+2]

# prime name and length of walk k
prime_sets = [
("p434",216),
("p441+",216),
("p503",250),
("p509+",250),
("p610",305),
("p619+",305),
("p751",372),
("p761+",372)]


def gen_csv(file_name):
	rows = [["proof_system","base_field","limbs","m","n"]]
	for (parameter,k) in prime_sets:
		if contains_plus(parameter):
			#if parameter is a plus prime,
			#run tests for fp, fp2 in both aurora, ligero and limbo
			rows.append(["ligero", parameter, limbs(parameter)] + get_n_m_p(k))
			rows.append(["aurora", parameter, limbs(parameter)] + get_n_m_p(k))
			rows.append(["limbo", parameter, limbs(parameter)] + get_n_m_p(k))
			rows.append(["ligero", parameter+"x", limbs(parameter)] + get_n_m_p2(k))
			rows.append(["aurora", parameter+"x", limbs(parameter)] + get_n_m_p2(k))
			rows.append(["limbo", parameter+"x", limbs(parameter)] + get_n_m_p2(k))
		else:
			#if parameter is a minus prime,
			#run fp tests for limbo
			rows.append(["limbo", parameter, limbs(parameter)] + get_n_m_p(k))
			#run fp2 tests for aurora, ligero, & limbo.
			rows.append(["ligero", parameter+"x", limbs(parameter)] + get_n_m_p2(k))
			rows.append(["aurora", parameter+"x", limbs(parameter)] + get_n_m_p2(k))
			rows.append(["limbo", parameter+"x", limbs(parameter)] + get_n_m_p2(k))
	with open(file_name, "w+") as file:
		csvfile = csv.writer(file, delimiter=',')
		csvfile.writerows(rows)

gen_csv("params.csv")
