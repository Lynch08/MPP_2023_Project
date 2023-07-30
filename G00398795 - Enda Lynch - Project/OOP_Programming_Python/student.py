class Student:
    def __init__(self, name, module_columns):
        self.name = name
        self.marks = {module: None for module in module_columns}

    def add_mark(self, module, mark):
        self.marks[module] = mark

    def calculate_gpa(self, gpa_scale):
        marks = list(self.marks.values())
        return gpa_scale.calculate_gpa(marks)

