/* tslint:disable:no-unused-variable */

import { TestBed, async, inject } from '@angular/core/testing';
import { BruimdbServiceService } from './bruimdb-service.service';

describe('Service: BruimdbService', () => {
  beforeEach(() => {
    TestBed.configureTestingModule({
      providers: [BruimdbServiceService]
    });
  });

  it('should ...', inject([BruimdbServiceService], (service: BruimdbServiceService) => {
    expect(service).toBeTruthy();
  }));
});
