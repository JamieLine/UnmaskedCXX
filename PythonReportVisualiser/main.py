from collections import defaultdict
import matplotlib.pyplot as plt
import csv
import typing

# TODO(linej): This should take in a file name rather than just looking for a single report.

# Small class to count passed and failed results later
class Results:
  def __init__(self):
    self.Passed = 0
    self.Failed = 0

  def AddResult(self, Res: str):
    if Res == "True":
      self.Passed += 1
    else: self.Failed += 1


# Now we use the Results class to partition the data by test category and type to plot later.
CategoriesAndResults = defaultdict(Results)
TypesAndResults = defaultdict(Results)

CATEGORY_CSV_INDEX = 1
TYPE_CSV_INDEX = 2
RESULT_CSV_INDEX = 3

with open("UnmaskedReport.csv", newline="") as CSVSource:
  CSVReader = csv.reader(CSVSource, delimiter=',')
  IsFirstRow = True
  for Row in CSVReader:
    if IsFirstRow:
      if Row != ["1"]:
        print("Error, the report should be a version 1 report for this script.")
        print(f"Version line was read in as {Row}")
      IsFirstRow = False

    else:
      CategoriesAndResults[Row[CATEGORY_CSV_INDEX]].AddResult(Row[RESULT_CSV_INDEX])
      TypesAndResults[Row[TYPE_CSV_INDEX]].AddResult(Row[RESULT_CSV_INDEX])



# Plot everything
for Category, Result in CategoriesAndResults.items():
  Figure, Axes = plt.subplots()

  # Reformat data to be kind to matplotlib
  Headers = ["Passed", "Failed"]
  Counts = [Result.Passed, Result.Failed]
  BarColours = ['tab:green', 'tab:red']

  print(Counts)

  Axes.bar(Headers, Counts, color=BarColours)

  if Category == "": Category = "Uncategorised"

  Axes.set_ylabel("Number of tests")
  Axes.set_title(f"{Category} Results")

  plt.show()
  