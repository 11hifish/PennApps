from fhirclient import client
settings = {
    'app_id': 'my_web_app',
    'api_base': 'https://fhir-open-api-dstu2.smarthealthit.org'
}
smart = client.FHIRClient(settings=settings)
import fhirclient.models.patient as p
patient = p.Patient.read('hca-pat-1', smart.server)
print(patient.birthDate.isostring)
# '1963-06-12'
print(smart.human_name(patient.name[0]))
# 'Christy Ebert'