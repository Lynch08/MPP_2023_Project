# This class will manage the functions that initialize, read and save the data from the CSV File
import pandas as pd

class CSVManager:
    def __init__(self, csv_file):
        self.csv_file = csv_file

    def read_data(self):
        return pd.read_csv(self.csv_file)

    def save_data(self, df):
        df.to_csv(self.csv_file, index=False)