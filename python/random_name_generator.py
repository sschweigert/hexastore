import random
import csv
import os
from sets import Set

def main():
	num_names_to_generate = 4000
	
	data_folder = os.path.join(os.pardir, "data")
	
	first_name_path = os.path.join(data_folder, "first_names.csv")	
	last_name_path = os.path.join(data_folder, "last_names.csv")	
	output_path = os.path.join(data_folder, "names.csv")

	first_names = build_name_list(first_name_path)
	last_names = build_name_list(last_name_path)
	
	full_names = randomly_join_names(first_names, last_names, num_names_to_generate)

	write_csv(output_path, full_names)	

def randomly_join_names(first_names, last_names, number):
	names = Set()
	while (len(names) < number):
		new_name = random.choice(first_names) + " " + random.choice(last_names)
		names.add(new_name)
	return names

def build_name_list(csv_filename):
	with open(csv_filename, 'rb') as csv_file:
		smallest_percentage = get_smallest_percentage(csv_file)	
		return create_weighted_name_list(csv_file, smallest_percentage)
		#return create_weighted_name_list(csv_file, 0.1)

def create_weighted_name_list(csv_file, smallest_weight):
	csv_file.seek(0)
	reader = csv.reader(csv_file, delimiter=",")
	names = []	
	for row in reader:
		num_times = int(float(row[1]) / smallest_weight)
		names.extend([row[0]] * num_times)

	return names
	
def get_smallest_percentage(csv_file):
	csv_file.seek(0)
	reader = csv.reader(csv_file, delimiter=",")
	min_percentage = 100
	
	for row in reader:
		min_percentage = min(min_percentage, float(row[1]))

	return min_percentage

def write_csv(filename, data_list):
	with open(filename, 'wb') as csvfile:
		writer = csv.writer(csvfile, delimiter=",")
		for row in data_list:
			writer.writerow([row])	

if __name__ == "__main__":
	main()
