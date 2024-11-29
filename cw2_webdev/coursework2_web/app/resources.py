from flask_restful import Resource

class YourResource(Resource):
    def get(self):
        return {'message': 'Hello, World!'}

    def post(self):
        # Handle POST request
        pass