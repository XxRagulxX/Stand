#include "Game/Names.hpp"
#include <cstdlib>

namespace Stand
{
	const char* const Names::last[NAMES_LAST_SIZE] = {
		"Smith",     "Johnson",   "Williams",  "Brown",     "Jones",     "Garcia",    "Miller",    "Davis",
		"Rodriguez", "Martinez",  "Hernandez", "Lopez",     "Gonzalez",  "Wilson",    "Anderson",  "Thomas",
		"Taylor",    "Moore",     "Jackson",   "Martin",    "Lee",       "Perez",     "Thompson",  "White",
		"Harris",    "Sanchez",   "Clark",     "Ramirez",   "Lewis",     "Robinson",  "Walker",    "Young",
		"Allen",     "King",      "Wright",    "Scott",     "Torres",    "Nguyen",    "Hill",      "Flores",
		"Green",     "Adams",     "Nelson",    "Baker",     "Hall",      "Rivera",    "Campbell",  "Mitchell",
		"Carter",    "Roberts",   "Gomez",     "Phillips",  "Evans",     "Turner",    "Diaz",      "Parker",
		"Cruz",      "Edwards",   "Collins",   "Reyes",     "Stewart",   "Morris",    "Morales",   "Murphy",
	};

	const char* const Names::male[NAMES_MALE_SIZE] = {
		"James",     "John",      "Robert",    "Michael",   "William",   "David",     "Richard",   "Joseph",
		"Thomas",    "Charles",   "Christopher","Daniel",   "Matthew",   "Anthony",   "Mark",      "Donald",
		"Steven",    "Paul",      "Andrew",    "Joshua",    "Kenneth",   "Kevin",     "Brian",     "George",
		"Timothy",   "Ronald",    "Edward",    "Jason",     "Jeffrey",   "Ryan",      "Jacob",     "Gary",
		"Nicholas",  "Eric",      "Jonathan",  "Stephen",   "Larry",     "Justin",    "Scott",     "Brandon",
		"Benjamin",  "Samuel",    "Raymond",   "Gregory",   "Frank",     "Alexander", "Patrick",   "Jack",
		"Dennis",    "Jerry",     "Tyler",     "Aaron",     "Jose",      "Adam",      "Nathan",    "Henry",
		"Douglas",   "Zachary",   "Peter",     "Kyle",      "Walter",    "Harold",    "Jeremy",    "Ethan",
	};

	const char* const Names::female[NAMES_FEMALE_SIZE] = {
		"Mary",      "Patricia",  "Jennifer",  "Linda",     "Barbara",   "Elizabeth", "Susan",     "Jessica",
		"Sarah",     "Karen",     "Lisa",      "Nancy",     "Betty",     "Margaret",  "Sandra",    "Ashley",
		"Dorothy",   "Kimberly",  "Emily",     "Donna",     "Michelle",  "Carol",     "Amanda",    "Melissa",
		"Deborah",   "Stephanie", "Rebecca",   "Sharon",    "Laura",     "Cynthia",   "Kathleen",  "Amy",
		"Angela",    "Shirley",   "Anna",      "Brenda",    "Pamela",    "Emma",      "Nicole",    "Helen",
		"Samantha",  "Katherine", "Christine", "Debra",     "Rachel",    "Carolyn",   "Janet",     "Catherine",
		"Maria",     "Heather",   "Diane",     "Julie",     "Joyce",     "Victoria",  "Ruth",      "Virginia",
		"Lauren",    "Kelly",     "Christina", "Joan",      "Evelyn",    "Judith",    "Megan",     "Andrea",
	};

	const char* Names::randFirstName(uint8_t gender)
	{
		if (gender == NAME_FEMALE)
			return female[static_cast<uint32_t>(rand()) % NAMES_FEMALE_SIZE];
		return male[static_cast<uint32_t>(rand()) % NAMES_MALE_SIZE];
	}

	const char* Names::randLastName()
	{
		return last[static_cast<uint32_t>(rand()) % NAMES_LAST_SIZE];
	}
}
