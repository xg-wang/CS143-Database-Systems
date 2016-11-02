import { Component, OnInit } from '@angular/core';
import { BruimdbServiceService } from '../../services/bruimdb-service.service';
import { FormData } from '../../models/form-data';

@Component({
  selector: 'app-add-people',
  templateUrl: './add-people.component.html',
  styleUrls: ['./add-people.component.css']
})
export class AddPeopleComponent implements OnInit {
  active = true;
  peopleForm = new FormData();
  entity: string;

  constructor(private _bruimdbService: BruimdbServiceService) { }

  onSubmit() {
    this._bruimdbService.addEntity({
      'entity': this.entity,
      'data': this.peopleForm
    }).subscribe(res => {
      // TODO: move this
      console.log(res);
    });
  }
  ngOnInit() {
  }

}
