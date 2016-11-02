import { Component, OnInit } from '@angular/core';
import { BruimdbServiceService } from '../../services/bruimdb-service.service';
import { PeopleForm } from '../../models/people-form';

@Component({
  selector: 'app-add-people',
  templateUrl: './add-people.component.html',
  styleUrls: ['./add-people.component.css']
})
export class AddPeopleComponent implements OnInit {
  active = true;
  peopleForm = new PeopleForm();

  constructor(private _bruimdbService: BruimdbServiceService) { }

  onSubmit() {
    this._bruimdbService.addEntity({
      'entity': this.peopleForm.entity,
      'data': this.peopleForm
    });
  }
  ngOnInit() {
  }

}
